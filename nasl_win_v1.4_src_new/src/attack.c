#include "includes.h"
#include "libnessus.h"

#include "nasl.h"
#include "pluginscheduler.h"
#include "report/report.h"
#include "attack.h"

#ifdef _XSCAN_PLUG
typedef VOID (CALLBACK *PALERT_USER) (char *szHostName, int nAlertType, char *szLine);
static PALERT_USER pAlertUser;
#endif

extern FILE *nasl_trace_fp;

int checks_script_timeout = INFINITE;
char checks_read_timeout[40] = "5";

int safe_checks_only = 0;
int disable_parrallel_checks = 0;

static CRITICAL_SECTION criPktLock;
static CRITICAL_SECTION criMemLock;
static int lock_inited = 0;

static thread_infos thread_list[MAX_THREADS+1] = {0};
static BOOL terminated = FALSE;

void init_lock()
{
    InitializeCriticalSection(&criPktLock);
    InitializeCriticalSection(&criMemLock);
    lock_inited = 1;
}

void set_pktlock()
{
    if (lock_inited) EnterCriticalSection(&criPktLock);
}

void unset_pktlock()
{
    if (lock_inited) LeaveCriticalSection(&criPktLock);
}

void set_memlock()
{
    if (lock_inited) EnterCriticalSection(&criMemLock);
}

void unset_memlock()
{
    if (lock_inited) LeaveCriticalSection(&criMemLock);
}

void release_lock()
{
    if (lock_inited) {
        lock_inited = 0;
        DeleteCriticalSection(&criPktLock);
        DeleteCriticalSection(&criMemLock);
    }
}

int is_safe_check()
{
    return safe_checks_only;
}

void set_timeout(int script_timeout, int network_timeout)
{
    if (script_timeout > 0) checks_script_timeout = script_timeout*1000;
    sprintf(checks_read_timeout, "%d", network_timeout);
}

void text_color(int color)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, color);
}

void pluginlaunch_disable_parrallel_checks()
{
    disable_parrallel_checks = 1;
}

void pluginlaunch_enable_parrallel_checks()
{
    disable_parrallel_checks = 0;
}

void plug_set_parameter(struct arglist *args, char *name, int type, void *value)
{
    struct arglist *prefs = arg_get_value(args, "preferences");

    if (!name || !value) return;

    if (!prefs) {
        prefs = emalloc(sizeof(struct arglist));
        arg_add_value(args, "preferences", ARG_ARGLIST, sizeof(struct arglist), prefs);
    }
    switch (type) {
    case ARG_STRING:
        if (arg_get_value(prefs, name))
            arg_set_value(prefs, name, strlen(value), estrdup(value));
        else
            arg_add_value(prefs, name, type, strlen(value), estrdup(value));
        break;
    case ARG_INT:
    case ARG_PTR:
        if (arg_get_value(prefs, name))
            arg_set_value(prefs, name, sizeof(int), value);
        else
            arg_add_value(prefs, name, type, sizeof(int), value);
        break;
    case ARG_ARGLIST:
        if (arg_get_value(prefs, name))
            arg_set_value(prefs, name, sizeof(struct arglist), value);
        else
            arg_add_value(prefs, name, type, sizeof(struct arglist), value);
        break;
    }
} 

void *plug_get_parameter(struct arglist *args, char *name)
{
    struct arglist * prefs = (struct arglist *)arg_get_value(args, "preferences");
    int type;

    if (!prefs) return(NULL);

    type = arg_get_type(prefs, name);
    if (type >= 0)
        return arg_get_value(prefs, name);

    return NULL;
}

int load_plugin_from_file(char *path, FILE *file, struct arglist *filelist)
{
    int plugin_num = 0;
    char szFileName[MAX_PATH] = {0};
    while (!feof(file)) {
        if (!fgets(szFileName, sizeof(szFileName)-1, file)) break;
        if (szFileName[strlen(szFileName)-1] == '\n') szFileName[strlen(szFileName)-1] = '\0';
        arg_add_value(filelist, szFileName, ARG_INT, sizeof(int), (void*)1);
        plugin_num++;
    }
    return plugin_num;
}

int load_plugin_from_dir(char *path, char *ext, struct arglist *filelist)
{
    int plugin_num = 0, i = 0;
    WIN32_FIND_DATA finddata;
    HANDLE hFind;
    char szLine[MAX_PATH] = {0};
    char szFullPath[MAX_PATH] = {0};
    char szFileName[MAX_PATH] = {0};
    char *ptr = NULL;

    if (!path || !ext || !filelist) return 0;

    strcpy(szFullPath, path);
    if (szFullPath[strlen(szFullPath)-1] != '\\') strcat(szFullPath, "\\");
    sprintf(szLine, "%s*", szFullPath);
    hFind = FindFirstFile(szLine, &finddata);
    if(hFind == INVALID_HANDLE_VALUE) return 0;

    do {
        if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(finddata.cFileName, ".") && strcmp(finddata.cFileName, "..")) {
                sprintf(szLine, "%s%s\\", szFullPath, finddata.cFileName);
                plugin_num += load_plugin_from_dir(szLine, ext, filelist);
            }
            continue;
        }
        ptr = strrchr(finddata.cFileName, '.');
        if (!ptr) continue;
        sprintf(szLine, "*%s", ptr);
        if (stricmp(szLine, ext) == 0) {
            sprintf(szLine, "%s", finddata.cFileName);
            arg_add_value(filelist, szLine, ARG_INT, sizeof(int), (void*)1);
            plugin_num++;
        }
    } while (FindNextFile(hFind, &finddata));
    FindClose(hFind);

    return plugin_num;
}

int load_plugins(char *path, char *ext, struct arglist *plugins, struct arglist *params, LOAD_PLUGIN_PROC load_plugin_proc)
{
    struct arglist *prefs, *filelist, *ptr;
    int plugin_num = 0, i = 0;
    FILE *test = NULL;
    char line[MAX_PATH];

    if (!plugins) return 0;
    if (!safe_checks_only) safe_checks_only = plug_get_parameter(params, "SkipDestructive");
#ifdef _XSCAN_PLUG
    get_work_dir(line, NULL);
    store_init_sys(line);
    if (path && !strlen(path)) strcpy(path, "scripts");
#else
    if (!path || !strlen(path)) return 0;
#endif

    filelist = emalloc(sizeof(struct arglist));
    test = fopen(path, "r");
    if (test) {
        plugin_num = load_plugin_from_file(path, test, filelist);
        fclose(test);
    }
    else
        plugin_num = load_plugin_from_dir(path, ext, filelist);
    ptr = filelist;
    i = 0;
    prefs = (struct arglist *)arg_get_value(params, "preferences");
    if (!prefs) {
        prefs = (struct arglist *)emalloc(sizeof(struct arglist));
        arg_add_value(params, "preferences", ARG_ARGLIST, sizeof(struct arglist), prefs);
    }
    while (ptr && ptr->name) {
        MSG msg;
        if (PeekMessage(&msg, 0, NULL, NULL, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) break;
        }
        nasl_plugin_add(ptr->name, plugins, prefs, LAUNCH_RUN);
        i++;
        if (load_plugin_proc) {
            int res = load_plugin_proc(i, plugin_num);
            if (res <= 0) break;
        }
        ptr = ptr->next;
    }
#ifndef _XSCAN_PLUG
    printf("\n\n");
#endif

    if (filelist) arg_free_all(filelist);
    
    return plugin_num;
}

void free_plugins(struct arglist *plugins)
{
    arg_free_all(plugins);
}

void get_local_os(char *szLocalOS)
{
    OSVERSIONINFO os; 

    os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
    GetVersionEx(&os);
    switch(os.dwPlatformId) { 
    case VER_PLATFORM_WIN32_WINDOWS: 
        sprintf(szLocalOS, "Windows %d.%d", os.dwMajorVersion, os.dwMinorVersion);
        break;
    case VER_PLATFORM_WIN32_NT: 
        sprintf(szLocalOS, "Windows NT %d.%d", os.dwMajorVersion, os.dwMinorVersion);
        break;
    default:
        sprintf(szLocalOS, "Windows NT *.*");
        break;
    } 
}

static VOID init_params(struct arglist *script_infos, struct arglist *params)
{
    if (!script_infos || !params) return;

    plug_set_parameter(script_infos, "port_range", ARG_STRING, plug_get_parameter(params, "Port/Range"));
    plug_set_parameter(script_infos, "log_file", ARG_STRING, plug_get_parameter(params, "LogFile"));
    plug_set_parameter(script_infos, "verbose", ARG_INT, plug_get_parameter(params, "ShowVerbose"));
    plug_set_parameter(script_infos, "max_threads", ARG_INT, plug_get_parameter(params, "MaxThreads"));
    plug_set_parameter(script_infos, "curr_threads", ARG_PTR, plug_get_parameter(params, "CurrentThreads"));
    plug_set_parameter(script_infos, "plugin_threads", ARG_PTR, plug_get_parameter(params, "PluginThreads"));

#ifdef _XSCAN_PLUG
    plug_set_parameter(script_infos, "Timeout", ARG_PTR,
        plug_get_parameter(params, "Timeout"));
    plug_set_parameter(script_infos, "VulnNumber", ARG_PTR,
        plug_get_parameter(params, "VulnNumber"));
    plug_set_parameter(script_infos, "CurrentSchedule", ARG_PTR,
        plug_get_parameter(params, "CurrentSchedule"));
    plug_set_parameter(script_infos, "CALLBACK/AlertUser", ARG_PTR,
        plug_get_parameter(params, "CALLBACK/AlertUser"));
    plug_set_parameter(script_infos, "CALLBACK/LogToFile", ARG_PTR,
        plug_get_parameter(params, "CALLBACK/LogToFile"));
    plug_set_parameter(script_infos, "CALLBACK/AddToTreeView", ARG_PTR,
        plug_get_parameter(params, "CALLBACK/AddToTreeView"));
#endif
}

static struct arglist *init_hostinfos(char *hostname, struct in_addr *ip)
{
    struct arglist * hostinfos;
    struct arglist * ports;

    hostinfos = emalloc(sizeof(struct arglist));
    arg_add_value(hostinfos, "FQDN", ARG_STRING, strlen(hostname), estrdup(hostname));
    arg_add_value(hostinfos, "NAME", ARG_STRING, strlen(hostname), estrdup(hostname));
    arg_add_value(hostinfos, "IP", ARG_PTR, sizeof(struct in_addr), ip);
    ports = emalloc(sizeof(struct arglist));
    arg_add_value(hostinfos, "PORTS", ARG_ARGLIST, sizeof(struct arglist), ports);  
    return(hostinfos);
}

struct arglist *init(struct arglist *script_infos, char *hostname, struct in_addr ip)
{
    struct arglist * prefs = NULL;
    struct in_addr * pip = emalloc(sizeof(*pip));
    *pip = ip;

    if (!script_infos) script_infos = emalloc(sizeof(struct arglist));

    prefs = arg_get_value(script_infos, "preferences");
    if (!prefs) {
        prefs = emalloc(sizeof(struct arglist));
        arg_add_value(script_infos, "preferences", ARG_ARGLIST, -1, prefs);
    }
    if (!arg_get_value(prefs, "checks_read_timeout"))
        arg_add_value(prefs, "checks_read_timeout", ARG_STRING, 1, estrdup(checks_read_timeout));

    if(safe_checks_only != 0) {
        if (!arg_get_value(prefs, "safe_checks"))
            arg_add_value(prefs, "safe_checks", ARG_STRING, 3, estrdup("yes"));
    }

    arg_add_value(script_infos, "HOSTNAME", ARG_ARGLIST, -1,
        init_hostinfos(hostname, pip));

    return script_infos;
}

void free_socket(struct arglist *script_infos)
{
    struct arglist *socket_list = NULL;
    struct arglist *temp_list = NULL;

    socket_list = (struct arglist *)arg_get_value(script_infos, "socket_list");
    if (!socket_list) return;

    temp_list = socket_list;
    while (temp_list) {
        if (temp_list->type == ARG_INT && temp_list->value >= 0)
            closesocket((int)temp_list->value);
        temp_list = temp_list->next;
    }
    arg_free_all(socket_list);
}

char *get_script_fullname(char *filename, char *full_pathname)
{
    if (!filename || !full_pathname) return NULL;

    strcpy(full_pathname, "\0");
    if (!strchr(filename, '/') && !strchr(filename, '\\')) {
        get_work_dir(full_pathname, NULL);
        if (strlen(full_pathname) && full_pathname[strlen(full_pathname)-1] != '\\') strcat(full_pathname, "\\");
    }
    strcat(full_pathname, filename);
    return full_pathname;
}

DWORD WINAPI check_threads(LPVOID lpParam)
{
    struct arglist *params = (struct arglist *)lpParam;
    char *target = plug_get_parameter(params, "HostName");
    int *pnCurrentThreads, *pnPluginThreads, *pnPluginNum;
    char szLine[1024] = {0};
    thread_params *thread_info = NULL;
    struct arglist *script_infos = NULL;
    struct scheduler_plugin *plugin = NULL;
    plugins_scheduler_t sched;
    DWORD dwExitCode = 0;
    int i = 0;

    while (!terminated) {
        for (i=0; i<MAX_THREADS; i++) {
            if (terminated) break;
            if (thread_list[i].start_time <= 0) continue;
            if (thread_list[i].timeout > 0 &&
                GetTickCount()-thread_list[i].start_time > thread_list[i].timeout) {

                set_pktlock();
                set_memlock();

                if (thread_list[i].timeout <= 0) {
                    unset_pktlock();
                    unset_memlock();
                    continue;
                }
                thread_list[i].timeout = 0;

#ifdef _XSCAN_PLUG
                GetString("ERR-SCRIPT-TIMEOUT", szLine, thread_list[i].script_name, NULL);
                pAlertUser = plug_get_parameter(params, "CALLBACK/AlertUser");
                pAlertUser(target, 0x103, szLine);
#else
                printf("Script \"%s\" timeout, terminated by force\n", thread_list[i].script_name);
#endif
                if (thread_list[i].thread_handle) {
                    GetExitCodeThread(thread_list[i].thread_handle, &dwExitCode);
                    TerminateThread(thread_list[i].thread_handle, dwExitCode);
                    CloseHandle(thread_list[i].thread_handle);
                    thread_list[i].thread_handle = 0;
                }
                thread_list[i].start_time = 0;

                pnCurrentThreads = plug_get_parameter(params, "CurrentThreads");
                pnPluginThreads = plug_get_parameter(params, "PluginThreads");
                pnPluginNum = plug_get_parameter(params, "plugin_num");

                thread_info = thread_list[i].thread_info;
                script_infos = thread_info->script_infos;
                plugin = thread_info->plugin;
                sched = thread_info->sched;

                if (pnCurrentThreads) (*pnCurrentThreads)--;
                if (pnPluginThreads) (*pnPluginThreads)--;
                if (pnPluginNum) (*pnPluginNum)--;

                plugin_set_running_state(sched, plugin, PLUGIN_STATUS_DONE);

                unset_pktlock();
                unset_memlock();

                if (script_infos) {
                    free_socket(script_infos);
                    arg_free_all(script_infos);
                    plugin->arglist->value = NULL;
                }
                if (thread_info) efree(&thread_info);
            }
            Sleep(1);
        }
        Sleep(10);
    }
    return 1;
}

DWORD WINAPI exec_script(LPVOID lpParam)
{
    int *pnCurrentThreads, *pnPluginThreads, *pnPluginNum, nVerbose;
    int nThreadIndex, i;
    thread_params *thread_info = (thread_params *)lpParam;
    struct arglist *params = thread_info->params;
    struct arglist *script_infos = thread_info->script_infos;
    plugins_scheduler_t sched = thread_info->sched;
    struct scheduler_plugin *plugin = thread_info->plugin;
    char full_pathname[MAX_PATH] = {0};
    char *filename = NULL, *target = NULL;
    char szLine[MAX_PATH] = {0};
    SYSTEMTIME StartTime, EndTime;

    pnCurrentThreads = plug_get_parameter(params, "CurrentThreads");
    pnPluginThreads = plug_get_parameter(params, "PluginThreads");
    pnPluginNum = plug_get_parameter(params, "plugin_num");
    nVerbose = plug_get_parameter(params, "ShowVerbose");

    filename = strrchr(thread_info->script_name, '\\');
    if (!filename) filename = strrchr(thread_info->script_name, '/');;
    if (filename) filename++;
    else filename = thread_info->script_name;

    target = plug_get_parameter(params, "HostName");
    if (nVerbose) {
        GetLocalTime(&StartTime);
        sprintf(szLine, "[*] Execute script \"%s\"", filename);
    }
    else {
        strcpy(szLine, "\0");
    }
#ifdef _XSCAN_PLUG
    if (nVerbose) {
        pAlertUser = plug_get_parameter(params, "CALLBACK/AlertUser");
        pAlertUser(target, 0, szLine);
    }
#else
    if (strlen(szLine)) printf("%s\n", szLine);
#endif

    get_script_fullname(thread_info->script_name, full_pathname);
    thread_info->mode |= NASL_ALWAYS_SIGNED;
    execute_nasl_script(thread_info->script_infos, full_pathname, NULL, thread_info->mode);

    nThreadIndex = -1;
    for (i=0; i<MAX_THREADS; i++) {
        if (terminated) break;
        if (stricmp(thread_list[i].script_name, filename) == 0) {
            nThreadIndex = i;
            break;
        }
    }
    if (nThreadIndex >= 0) {
        set_memlock();
        if (thread_list[nThreadIndex].timeout <= 0) {
            unset_memlock();
            return 0;
        }
        thread_list[nThreadIndex].timeout = 0;
        if (thread_list[nThreadIndex].thread_handle) {
            CloseHandle(thread_list[nThreadIndex].thread_handle);
            thread_list[nThreadIndex].thread_handle = 0;
        }
        thread_list[nThreadIndex].start_time = 0;
        unset_memlock();
    }
    else {
#ifdef _DEBUG
        MessageBox(0, "Failed to lookup self", "Error", MB_OK);
#endif
    }

    if (nVerbose) {
        GetLocalTime(&EndTime);
        sprintf(szLine, "[*] \"%s\" completed, %d/%d/%d/%d:%d:%d - %d/%d/%d/%d:%d:%d",
            filename,
            StartTime.wYear, StartTime.wMonth, StartTime.wDay, StartTime.wHour, StartTime.wMinute, StartTime.wSecond,
            EndTime.wYear, EndTime.wMonth, EndTime.wDay, EndTime.wHour, EndTime.wMinute, EndTime.wSecond);
#ifdef _XSCAN_PLUG
        pAlertUser = plug_get_parameter(params, "CALLBACK/AlertUser");
        pAlertUser(target, 0, szLine);
#else
        printf("%s\n", szLine);
#endif
    }

    if (pnCurrentThreads) (*pnCurrentThreads)--;
    if (pnPluginThreads) (*pnPluginThreads)--;
    if (pnPluginNum) (*pnPluginNum)--;

    plugin_set_running_state(sched, plugin, PLUGIN_STATUS_DONE);

    if (script_infos) {
        free_socket(script_infos);
        arg_free_all(script_infos);
        plugin->arglist->value = NULL;
    }

    if (thread_info) efree(&thread_info);

    return 0;
}

void add_thread(thread_params *thread_info)
{
    int *pnCurrentThreads, *pnPluginThreads, *pnPluginNum, nVerbose, nTimeout;
    int nThreadIndex, i;
    struct arglist *params = thread_info->params;
    struct arglist *script_infos = thread_info->script_infos;
    char *filename = NULL;
    struct in_addr ip;
    struct hostent *hp;
    unsigned long ul;
    char szLine[1024] = {0};

    HANDLE hThread;
    DWORD dwThreadId, dwExitCode;

    pnCurrentThreads = plug_get_parameter(params, "CurrentThreads");
    pnPluginThreads = plug_get_parameter(params, "PluginThreads");
    pnPluginNum = plug_get_parameter(params, "plugin_num");
    nVerbose = plug_get_parameter(params, "ShowVerbose");

    filename = strrchr(thread_info->script_name, '\\');
    if (!filename) filename = strrchr(thread_info->script_name, '/');;
    if (filename) filename++;
    else filename = thread_info->script_name;

    nTimeout = plug_get_timeout(script_infos);
    if (nTimeout <= 0) nTimeout = checks_script_timeout;
    else {
#ifdef _DEBUG
        printf("%s: script timeout will be set to %ds\n", thread_info->script_name, nTimeout);
#endif
        nTimeout *= 1000;
    }

    set_memlock();

    nThreadIndex = -1;
    for (i=0; i<MAX_THREADS; i++) {
        if (terminated) break;
        if (thread_list[i].start_time <= 0) {
            nThreadIndex = i;
            break;
        }
    }
    if (nThreadIndex >= 0) {
        thread_list[nThreadIndex].script_name = filename;
        thread_list[nThreadIndex].start_time = GetTickCount();
        thread_list[nThreadIndex].timeout = nTimeout;
        thread_list[nThreadIndex].thread_info = thread_info;
        hThread = CreateThread(NULL, 0, exec_script, thread_info, 0, &dwThreadId);
        thread_list[nThreadIndex].thread_handle = hThread;
    }
    else {
        (*pnCurrentThreads)--;
        (*pnPluginThreads)--;
#ifdef _DEBUG
        MessageBox(0, "Failed to add new thread", "Error", MB_OK);
#endif
    }

    unset_memlock();

    return 0;
}

int check_host_summary(char *target, char *port_range, struct arglist *params)
{
    char *scripts_dir, *scan_mode, *log_file, *curr_task, *ptr;
    struct arglist *script_infos = NULL;
    struct kb_item **kb;
    struct arglist *key;
    struct in_addr ip;
    struct hostent *hp;
    unsigned long ul;
    int port_num = 0, local_host = 0;
    SYSTEMTIME StartTime, EndTime;
    char szLine[1024] = {0};
    int type = 0, i = 0, verbose = 0;

    int nMaxThreads = 0, *pnCurrentThreads, *pnPluginThreads, *pnTimeout;

    scan_mode = plug_get_parameter(params, "Port/ScanMode");
    log_file = plug_get_parameter(params, "LogFile");
    curr_task = plug_get_parameter(params, "CurrentSchedule");
    verbose = plug_get_parameter(params, "ShowVerbose");
    nMaxThreads = plug_get_parameter(params, "MaxThreads");
    pnCurrentThreads = plug_get_parameter(params, "CurrentThreads");
    pnPluginThreads = plug_get_parameter(params, "PluginThreads");
    pnTimeout = plug_get_parameter(params, "Timeout");

    ul = inet_addr(target);
    if(ul == 0xffffffff) {
        if((hp = gethostbyname(target)))
            memcpy(&ip, hp->h_addr, hp->h_length);
    }
    else memcpy(&ip, &ul, sizeof(ip));

    if (islocalhost(&ip)) local_host = 1;

    kb = plug_get_kb(params);
    if (!kb) {
        kb = kb_new();
        arg_add_value(params, "key", ARG_PTR, -1, kb);
    }

    // port-scan
    script_infos = init(NULL, target, ip);
    arg_add_value(script_infos, "key", ARG_PTR, -1, kb);
#ifndef _XSCAN_PLUG
    printf("[*] PORT-SCAN\n");
#endif
    if (verbose) GetLocalTime(&StartTime);
#ifdef _XSCAN_PLUG
    GetString("PORT-SCANNING-PORT", szLine, NULL, NULL);
    if (curr_task) sprintf(curr_task, "%s", szLine);
    if (verbose) {
        pAlertUser = plug_get_parameter(params, "CALLBACK/AlertUser");
        pAlertUser(target, 0, szLine);
    }
    if (pnTimeout) *pnTimeout = 0;
#endif
    port_plugin_init(script_infos);
    init_params(script_infos, params);
    if (!scan_mode) scan_mode = strdup("tcp");
    if (local_host) plug_set_parameter(script_infos, "scan_method", ARG_STRING, "fasttcp");
    else plug_set_parameter(script_infos, "scan_method", ARG_STRING, scan_mode);
    plug_set_parameter(script_infos, "Port/IdentifyService", ARG_INT,
        plug_get_parameter(params, "Port/IdentifyService"));
    port_num = port_plugin_run(script_infos);
#ifndef _XSCAN_PLUG
    if (verbose) {
        GetLocalTime(&EndTime);
        printf("[*] Completed, %d/%d/%d/%d:%d:%d - %d/%d/%d/%d:%d:%d\n",
            StartTime.wYear, StartTime.wMonth, StartTime.wDay, StartTime.wHour, StartTime.wMinute, StartTime.wSecond,
            EndTime.wYear, EndTime.wMonth, EndTime.wDay, EndTime.wHour, EndTime.wMinute, EndTime.wSecond);
    }
#endif
    arg_free_all(script_infos);

    // find-service-plug-in
    if (plug_get_parameter(params, "Port/IdentifyService")) {
        script_infos = init(NULL, target, ip);
        arg_add_value(script_infos, "key", ARG_PTR, -1, kb);
#ifndef _XSCAN_PLUG
        printf("[*] FIND-SERVICES\n");
#endif
        if (verbose) GetLocalTime(&StartTime);
#ifdef _XSCAN_PLUG
        GetString("PORT-SCANNING-IDENTIFY-SERVICE", szLine, NULL, NULL);
        if (curr_task) sprintf(curr_task, "%s", szLine);
        if (verbose) {
            pAlertUser = plug_get_parameter(params, "CALLBACK/AlertUser");
            pAlertUser(target, 0, szLine);
        }
        if (pnTimeout) *pnTimeout = 0;
#endif
        find_service_plugin_init(script_infos);
        init_params(script_infos, params);
        find_service_plugin_run(script_infos);
#ifndef _XSCAN_PLUG
        if (verbose) {
            GetLocalTime(&EndTime);
            printf("[*] Completed, %d/%d/%d/%d:%d:%d - %d/%d/%d/%d:%d:%d\n",
                StartTime.wYear, StartTime.wMonth, StartTime.wDay, StartTime.wHour, StartTime.wMinute, StartTime.wSecond,
                EndTime.wYear, EndTime.wMonth, EndTime.wDay, EndTime.wHour, EndTime.wMinute, EndTime.wSecond);
        }
#endif
        arg_free_all(script_infos);
    }

#ifdef _DEBUG
    key = plug_get_oldstyle_kb(params);
    if (key) {
        text_color(15);
        printf("\n\nFindService() kb_list:\n");
        arg_dump(key, 0);
        text_color(7);
    }
#endif

    return port_num;
}

int start_attack(struct arglist *params)
{
    char *target, *scripts_dir, *include_dir, *port_range, *scan_mode, *trace_file, *log_file, *curr_task;
    struct arglist *script_infos = NULL, *plugins = NULL, *prefs = NULL, *key, *argtmp;
    plugins_scheduler_t sched;
    struct in_addr ip;
    int i = 0, exec_mode = 0, verbose = 0;
    FILE *fp_trace = NULL;

    struct hostent *hp;
    unsigned long ul;
    int local_host = 0, plugin_num = 0, total_plugin = 0, *vuln_num;
    struct kb_item **kb;
    struct scheduler_plugin *plugin = NULL;
    struct arglist *templist = NULL;
    SYSTEMTIME StartTime, EndTime;
    char szPath[MAX_PATH] = {0};
    char szLine[1024] = {0};
    char *ptr = NULL;

    int nMaxThreads = 0, nMaxPluginThreads = 0, *pnCurrentThreads, *pnPluginThreads, *pnTimeout;
    HANDLE hThread = NULL;
    DWORD dwThreadId = 0;
    thread_params *thread_info = NULL;
    BOOL local_load_plugins = FALSE;

    target = plug_get_parameter(params, "HostName");
    scripts_dir = plug_get_parameter(params, "ScriptsDir");
    include_dir = plug_get_parameter(params, "IncludeDir");
    port_range = plug_get_parameter(params, "Port/Range");
    scan_mode = plug_get_parameter(params, "Port/ScanMode");
    exec_mode = plug_get_parameter(params, "ExecMode");
    trace_file = plug_get_parameter(params, "TraceFile");
    log_file = plug_get_parameter(params, "LogFile");
    curr_task = plug_get_parameter(params, "CurrentSchedule");
    verbose = plug_get_parameter(params, "ShowVerbose");
    plugins = plug_get_parameter(params, "ScriptList");
    plugin_num = plug_get_parameter(params, "ScriptNumber");
    nMaxThreads = plug_get_parameter(params, "MaxThreads");
    GetPrivateProfileString("Plugin-Threads", "-nasl", "-1", szLine, sizeof(szLine)-1,
        ".\\dat\\config.ini");
    nMaxPluginThreads = atoi(szLine);
    if (nMaxPluginThreads <= 0) nMaxPluginThreads = nMaxThreads;
    pnCurrentThreads = plug_get_parameter(params, "CurrentThreads");
    pnPluginThreads = plug_get_parameter(params, "PluginThreads");
    pnTimeout = plug_get_parameter(params, "Timeout");

    if (!safe_checks_only) safe_checks_only = plug_get_parameter(params, "SkipDestructive");
    ptr = plug_get_parameter(params, "Network/Adapter");
    if (ptr) {
        strcpy(szLine, ptr);
        ptr = strstr(szLine, " (");
        if (ptr) ptr[0] = '\0';
        setif_byname(szLine);
#ifdef _DEBUG
        printf("Network Interface: %s\n", szLine);
#endif
    }

    if (trace_file && strlen(trace_file)) {
        if (strcmp(trace_file, "-") == 0) {
            verbose = 1;
            nasl_trace_fp = stderr;
        }
        else {
            fp_trace = fopen(trace_file, "w");
            if (fp_trace == NULL) {
                perror(trace_file);
                return 0;
            }
            nasl_trace_fp = fp_trace;
        }
    }

    GetModuleFileName(0, szPath, sizeof(szPath)-1);
    ptr = strrchr(szPath, '\\');
    if (ptr) ptr[0] = '\0';
    
    if (scripts_dir && !include_dir) include_dir = scripts_dir;
    set_work_dir(scripts_dir, include_dir);

    ul = inet_addr(target);
    if(ul == 0xffffffff) {
        if((hp = gethostbyname(target)))
            memcpy(&ip, hp->h_addr, hp->h_length);
    }
    else memcpy(&ip, &ul, sizeof(ip));

    if (islocalhost(&ip)) local_host = 1;

    kb = plug_get_kb(params);
    if (!kb) kb = kb_new();

    get_work_dir(szLine, NULL);
    store_init_sys(szLine);
    if (!plugins) {
        if (curr_task) sprintf(curr_task, "%s", "Loading scripts ...");
        plugins = (struct arglist *)emalloc(sizeof(struct arglist));
        prefs = arg_get_value(params, "preferences");
        if (!prefs) {
            prefs = emalloc(sizeof(struct arglist));
            arg_add_value(params, "preferences", ARG_ARGLIST, sizeof(struct arglist), prefs);
        }
        get_work_dir(szLine, NULL);
        plugin_num = load_plugins(szLine, "*.nasl", plugins, prefs, NULL);
        local_load_plugins = TRUE;
    }

    ul = inet_addr(target);
    if(ul == 0xffffffff) {
        if((hp = gethostbyname(target)))
            memcpy(&ip, hp->h_addr, hp->h_length);
    }
    else memcpy(&ip, &ul, sizeof(ip));

    sched  = plugins_scheduler_init(plugins, 0, 1);
    
    plugin_num = 0; argtmp = plugins;
    while (argtmp && argtmp->next) {
        plugin_num++;
        argtmp = argtmp->next;
    }
    total_plugin = plugin_num;
    plug_set_parameter(params, "plugin_num", ARG_PTR, &plugin_num);

    if (total_plugin <= 0) {
        printf("No plug-in be found\n");
        return 0;
    }
#ifndef _XSCAN_PLUG
    printf("Total %d scripts.\n", total_plugin);
#endif

    init_lock();
    terminated = FALSE;
    memset(thread_list, 0, sizeof(thread_list));
    hThread = CreateThread(NULL, 0, check_threads, params, 0, &dwThreadId);
    if (hThread) CloseHandle(hThread);

    while (plugin_num > 0) {
#ifndef _XSCAN_PLUG
        if (!verbose) printf("%d%%\r", (total_plugin-plugin_num)*100/total_plugin);
#endif
        plugin = plugins_scheduler_next(sched);
        if (plugin != NULL && plugin != PLUG_RUNNING) { 

            script_infos = plugin->arglist->value;
            if (!arg_get_value(script_infos, "preferences")) {
                arg_add_value(script_infos, "preferences", ARG_PTR, -1, arg_get_value(params, "preferences"));
            }
            if (!arg_get_value(script_infos, "HOSTNAME")) {
                init(script_infos, target, ip);
                arg_add_value(script_infos, "key", ARG_PTR, -1, kb);
            }
            if (!plug_get_parameter(script_infos, "port_range")) {
                init_params(script_infos, params);
            }
            if (!requirements_plugin(kb, plugin, arg_get_value(script_infos, "preferences"))) {
#ifdef _XSCAN_PLUG
                GetString("NASL-EXECUTING-SCRIPT", szLine, plugin->arglist->name, NULL);
                if (curr_task) sprintf(curr_task, "%s (%d/%d)", szLine, total_plugin-plugin_num, total_plugin);
                if (pnTimeout) *pnTimeout = 0;
#endif
                if (disable_parrallel_checks) {
                    while ((*pnPluginThreads) >= 1) Sleep(1);
                }
                while ((*pnCurrentThreads) >= nMaxThreads ||
                    (*pnPluginThreads) >= nMaxPluginThreads)
                    Sleep(10);

                (*pnCurrentThreads)++;
                (*pnPluginThreads)++;

                thread_info = (thread_params *)emalloc(sizeof(thread_params));
                thread_info->sched = sched;
                thread_info->plugin = plugin;
                thread_info->params = params;
                thread_info->kb = kb;
                thread_info->script_infos = script_infos;
                thread_info->script_name = plugin->arglist->name;
                thread_info->mode = exec_mode;
                add_thread(thread_info);
            }
            else {
                plugin_set_running_state(sched, plugin, PLUGIN_STATUS_DONE);
                plugin_num--;
                arg_free_all(plugin->arglist->value);
                plugin->arglist->value = NULL;
            }

        }
        else if(plugin == NULL) {
            if (plugin_num > 0) Sleep(10);
            else break;
        }
    }

    terminated = TRUE;
    release_lock();

#ifndef _XSCAN_PLUG
    if (!verbose) printf("100%%\n", (total_plugin-plugin_num)*100/total_plugin);
#endif

    if (kb && !plug_get_kb(params))
        arg_add_value(params, "key", ARG_PTR, -1, kb);

#ifdef _DEBUG
    key = plug_get_oldstyle_kb(params);
    if (key) {
        text_color(15);
        printf("\n\nStartAttack() kb_list:\n");
        arg_dump(key, 0);
        text_color(7);
    }
#endif

    vuln_num = (int *)plug_get_parameter(params, "VulnNumber");

    // free global var

    if (local_load_plugins) {
        free_plugins(plugins);
#ifdef _DEBUG
        printf("free_plugins(plugins) complete.\n");
#endif
    }
    plugins_scheduler_free(sched);
#ifdef _DEBUG
    printf("plugins_scheduler_free(sched) complete.\n");
#endif

    if (vuln_num)
        return *vuln_num;
    else
        return 0;
}