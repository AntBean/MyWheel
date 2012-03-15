////////////////////////////////////////////////////////////////////////
//                                                                    //
//  #includes                                                         //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "../src/includes.h"
#include "../src/libnessus.h"

#include "../src/nasl.h"
#include "../src/plugutils.h"
#include "../src/nasl_nessusd_glue.h"
#include "../src/attack.h"
#include "../src/tcp.h"
#include "../src/report/report.h"

#ifdef __cplusplus
    }
#endif // __cplusplus

#include <pcap.h>
#include <packet32.h>

#pragma comment(lib, "ws2_32.lib")  // for win socket
#pragma comment(lib, "wpcap.lib")   // for winpcap library
#pragma comment(lib, "packet.lib")  // for winpcap library
    
////////////////////////////////////////////////////////////////////////
//                                                                    //
//  #global define                                                    //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#define IT_NOTE             0x110   //提示
#define IT_WARN             0x111   //警告
#define IT_HOLE             0x112   //漏洞

typedef VOID (CALLBACK *PALERT_USER) (
    char *szHostName,
    int nType,
    char *szLine);

typedef VOID (CALLBACK *PLOG_TO_FILE) (
    char *szLogFile,
    char *szHost,
    char *szPort,
    char *szDataType,
    char *szData);

typedef VOID (CALLBACK *PADD_TO_TREEVIEW) (
    char *szLine,
    char *szImageFile);

static PALERT_USER      pAlertUser = NULL;      //回调函数"AlertUser()"地址
static PLOG_TO_FILE     pLogToFile = NULL;      //回调函数"LogToFile()"地址
static PADD_TO_TREEVIEW pAddToTreeView = NULL;  //回调函数"AddToTreeView()"地址
static struct arglist    *dup_params = NULL;

extern "C" __declspec(dllexport) void __stdcall GetNaslVersion(char *);
extern "C" __declspec(dllexport) void __stdcall SetLanguage(struct arglist *, char *);
extern "C" __declspec(dllexport) void __stdcall SetTimeout(int, int);
extern "C" __declspec(dllexport) void __stdcall SetWorkDirectory(char *, char *);
extern "C" __declspec(dllexport) void __stdcall GetWorkDirectory(char *, char *);
extern "C" __declspec(dllexport) struct arglist * __stdcall LoadPlugins(struct arglist *, char *, char *, int *, LOAD_PLUGIN_PROC);
extern "C" __declspec(dllexport) struct arglist * __stdcall LoadPluginsFromMemMap(struct arglist *, char *, int, int *);
extern "C" __declspec(dllexport) void __stdcall FreePlugins(struct arglist *);
extern "C" __declspec(dllexport) struct arglist * __stdcall LoadPluginInfo(char *filename, struct arglist *pref);
extern "C" __declspec(dllexport) int __stdcall ExecNaslScript(struct arglist *, char *, int);
extern "C" __declspec(dllexport) int __stdcall FindService(char *, char *, struct arglist *);
extern "C" __declspec(dllexport) int __stdcall StartAttack(struct arglist *params);
extern "C" __declspec(dllexport) int __stdcall BuildReport(char *, char *, int);
extern "C" __declspec(dllexport) int __stdcall GetIfList(char *, GET_IF_PROC);

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  #functions                                                        //
//                                                                    //
////////////////////////////////////////////////////////////////////////

void __stdcall GetNaslVersion(char *version)
{
    strcpy(version, nasl_version());
}

void __stdcall SetLanguage(struct arglist *script_infos, char *language)
{
    if (!script_infos || !language) return;

    struct arglist *prefs = (struct arglist *)arg_get_value(script_infos, "preferences");

    if (!prefs) {
        prefs = (struct arglist *)emalloc(sizeof(struct arglist));
        arg_add_value(script_infos, "preferences", ARG_ARGLIST, sizeof(struct arglist), prefs);
    }
    if (arg_get_value(prefs, "language"))
        arg_set_value(prefs, "language", strlen(language), estrdup(language));
    else
        arg_add_value(prefs, "language", ARG_STRING, strlen(language), estrdup(language));
    
    set_language(language);
}

void __stdcall SetTimeout(int script_timeout, int network_timeout)
{
    set_timeout(script_timeout, network_timeout);
}

void __stdcall SetWorkDirectory(char *scripts, char *include)
{
    set_work_dir(scripts, include);
}

void __stdcall GetWorkDirectory(char *scripts, char *include)
{
    get_work_dir(scripts, include);
}

struct arglist * __stdcall LoadPluginInfo(char *filename, struct arglist *pref)
{
    return nasl_plugin_get_info(filename, pref);
}

struct arglist * __stdcall LoadPlugins(struct arglist *params, char *path, char *ext, int *num, LOAD_PLUGIN_PROC load_plugin_proc)
{
    struct arglist *plugins = (struct arglist *)emalloc(sizeof(struct arglist));
    struct arglist *prefs = NULL;

    if (dup_params) {
        arg_free_all(dup_params);
        dup_params = NULL;
    }
    dup_params = (struct arglist *)emalloc(sizeof(struct arglist));
    if (params) {
        arg_dup(dup_params, params);
    }

    if (plugins) {
        prefs = (struct arglist *)arg_get_value(dup_params, "preferences");
        if (!prefs) {
            prefs = (struct arglist *)emalloc(sizeof(struct arglist));
            arg_add_value(dup_params, "preferences", ARG_ARGLIST, sizeof(struct arglist), prefs);
        }
        (*num) = load_plugins(path, ext, plugins, dup_params, load_plugin_proc);
    }

    return plugins;
}

void __stdcall FreePlugins(struct arglist *plugins)
{
    free_plugins(plugins);
}

int __stdcall ExecNaslScript(struct arglist *script_infos, char *filename, int mode)
{
    if (!script_infos || !filename) return 0;
    
    return execute_nasl_script(script_infos, filename, ".", mode);
}

int __stdcall FindService(char *target, char *port_range, struct arglist *params)
{
    return check_host_summary(target, port_range, params);
}

int __stdcall StartAttack(struct arglist *params)
{
    int nRtn = 0;

    tcp_init();

    nRtn = start_attack(params);

    tcp_exit();

    return nRtn;
}

int __stdcall BuildReport(char *log, char *report, int type)
{
    if (!log || !strlen(log)) return -1;
    
    if (!report || !strlen(report)) report = log;

    if (!type) {
        char *ptr = strrchr(report, '.');
        if (ptr) {
            ptr++;
            if (!stricmp(ptr, "txt"))
                type = SAVE_TEXT;
            else if (!stricmp(ptr, "htm") || !stricmp(ptr, "html"))
                type = SAVE_HTML;
            else if (!stricmp(ptr, "xml"))
                type = SAVE_XML;
            else
                type = SAVE_TEXT;
        }
    }
    struct arglist *result = log_to_arglist(log);
    save_report(report, type, result);
    efree(&result);

    return 0;
}

int __stdcall GetIfList(char *szWinPCapVer, GET_IF_PROC get_if_proc)
{
    pcap_if_t *alldevs;
    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE+1];
    char szLine[1024] = {0};
    char szFullLine[1024] = {0};
    int i = 0;

    sprintf(szWinPCapVer, "%s", PacketGetVersion());

    // Retrieve the interfaces list 
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return 0;
    }

    // Scan the list printing every entry 
    for (i=0,d=alldevs; d; i++,d=d->next) {
        pcap_addr_t *a;
        for (a=d->addresses; a; a=a->next) {
            if (a->addr->sa_family == AF_INET) {
                sprintf(szFullLine, "%s (%s)", d->name, iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
                get_if_proc(szFullLine);
            }
        }
    }

    return i;
}

HANDLE InitMemMap(char *szFileName, int nFileSize, char *szMapName)
{
    //创建/打开物理文件
    HANDLE hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    //映射文件
    HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0,
        nFileSize, szMapName);
    
    CloseHandle(hFile);
    if (!hFileMap) {
        fprintf(stderr, "CreateFileMapping() failed, error code: %lu\n", GetLastError());
        CloseHandle(hFileMap);
        return NULL;
    }

    return hFileMap;
}

void *MapMem(HANDLE hFileMap, int nOffset)
{
    if (!hFileMap) return NULL;

    //映射文件
    char *pMem = (char *)MapViewOfFile(hFileMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);
    if (!pMem) {
        fprintf(stderr, "MapViewOfFile() failed, error code: %lu\n", GetLastError());
        CloseHandle(hFileMap);
        UnmapViewOfFile(pMem);
        return NULL;
    }
    return pMem+nOffset;
}

void FreeMemMap(HANDLE hFileMap)
{
    CloseHandle(hFileMap);
}

typedef struct _NASL_SCRIPT_INFO {
    int id;
    int category;
    char name[128];
} NASL_SCRIPT_INFO;

struct arglist * __stdcall LoadPluginsFromMemMap(struct arglist *params, char *filename, int filesize, int *num)
{
    int i = 0;
    struct arglist *scripts = NULL;
    struct arglist *prefs = NULL;

    if (dup_params) {
        arg_free_all(dup_params);
        dup_params = NULL;
    }
    dup_params = (struct arglist *)emalloc(sizeof(struct arglist));
    if (params) {
        arg_dup(dup_params, params);
    }

    // 载入NASL参数
    prefs = (struct arglist *)arg_get_value(dup_params, "preferences");
    if (!prefs) {
        prefs = (struct arglist *)emalloc(sizeof(struct arglist));
        arg_add_value(dup_params, "preferences", ARG_PTR, -1, prefs);
    }

    // 载入NASL脚本
    if (strlen(filename) && filesize>0) {
        HANDLE hMapFile = InitMemMap(filename, filesize, "NASL_SCRIPTS");
        if (!hMapFile) return NULL;

        char *ptr = (char *)MapMem(hMapFile, 0);
        int nNum = (int)plug_get_parameter(dup_params, "ScriptNumber");
        scripts = (struct arglist *)emalloc(sizeof(struct arglist));
        *(num) = 0;
        for (i=0; i<nNum; i++) {
            struct arglist *args = (struct arglist *)emalloc(sizeof(arglist));
            NASL_SCRIPT_INFO *script_info = (NASL_SCRIPT_INFO *)ptr;

            if (script_info->id <= 0) continue;
            if (arg_get_value(scripts, script_info->name)) continue;

            plug_set_id(args, script_info->id);
            plug_set_category(args, script_info->category);
            plug_set_fname(args, script_info->name);
            plug_set_launch(args, 1);
            arg_add_value(args, "preferences", ARG_PTR, -1, prefs);

            arg_add_value(scripts, script_info->name, ARG_ARGLIST, -1, args);
            ptr += sizeof(NASL_SCRIPT_INFO);
            *(num)++;
        }
        FreeMemMap(hMapFile); hMapFile = NULL;
    }
    else scripts = LoadPlugins(dup_params, "scripts", "*.nasl", num, NULL);

    return scripts;
}

extern "C" VOID PutHostAlert(char *szHostName, int nType, char *szLine)
{
    if (pAlertUser) {
        pAlertUser(szHostName, nType, szLine);
    }
}

extern "C" VOID PutHostTreeView(char *szHostName, char *szLine, char *szIcon)
{
    char szFullLine[MAX_PATH] = {0};

    sprintf(szFullLine, "%s\n%s", szHostName, szLine);
    if (pAddToTreeView) {
        pAddToTreeView(szFullLine, szIcon);
    }
}

extern "C" VOID PutAlert(struct arglist *desc, int nType, char *szLine, char *szIcon)
{
    if (!pAlertUser)
        pAlertUser = (PALERT_USER)plug_get_parameter(desc, "CALLBACK/AlertUser");
    if (!pLogToFile)
        pLogToFile = (PLOG_TO_FILE)plug_get_parameter(desc, "CALLBACK/LogToFile");
    if (!pAddToTreeView)
        pAddToTreeView = (PADD_TO_TREEVIEW)plug_get_parameter(desc, "CALLBACK/AddToTreeView");

    struct arglist *hinfos = (struct arglist *)arg_get_value(desc, "HOSTNAME");
    if (hinfos) {
        PutHostAlert((char *)arg_get_value(hinfos, "NAME"), nType, szLine);
    }
}

extern "C" VOID PutTreeView(struct arglist *desc, int nType, char *szLine)
{
    if (!pAlertUser)
        pAlertUser = (PALERT_USER)plug_get_parameter(desc, "CALLBACK/AlertUser");
    if (!pLogToFile)
        pLogToFile = (PLOG_TO_FILE)plug_get_parameter(desc, "CALLBACK/LogToFile");
    if (!pAddToTreeView)
        pAddToTreeView = (PADD_TO_TREEVIEW)plug_get_parameter(desc, "CALLBACK/AddToTreeView");

    char szIcon[MAX_PATH] = {0};
    switch (nType) {
    case IT_NOTE:
        sprintf(szIcon, "%s", "info.bmp");
        break;
    case IT_WARN:
        sprintf(szIcon, "%s", "low.bmp");
        break;
    case IT_HOLE:
        sprintf(szIcon, "%s", "high.bmp");
        break;
    default:
        sprintf(szIcon, "%s", "info.bmp");
        break;
    }
    
    char szBuf[1024] = {0};
    strcpy(szBuf, szLine);
    char *ptr = strstr(szBuf, " - ");
    if (ptr) {
        ptr[0] = '\0'; ptr += strlen(" - ");
    }

    struct arglist *hinfos = (struct arglist *)arg_get_value(desc, "HOSTNAME");
    if (hinfos) {
        if (ptr) {
            PutHostTreeView((char *)arg_get_value(hinfos, "NAME"), szBuf, "port.bmp");
            strcat(szBuf, "\n"); strcat(szBuf, ptr);
        }
        PutHostTreeView((char *)arg_get_value(hinfos, "NAME"), szBuf, szIcon);
    }
}

extern "C" char *GetString(char *szName, char *szLine, char *szKey1, char *szKey2)
{
    char szTempLine[10240+1] = {0}, szBuf[10240+1] = {0};
    char szBuf1[1024+1] = {0}, szBuf2[1024+1] = {0};
    char *ptr = NULL;

    char szLanguage[MAX_PATH+1] = {0};
    char szLangFile[MAX_PATH+1] = {0};
    char szDataFile[MAX_PATH+1] = {0};

    GetModuleFileName(0, szBuf, MAX_PATH);
    ptr = strrchr(szBuf, '\\');
    if (ptr) ptr[0] = '\0';
    sprintf(szDataFile, "%s\\%s", szBuf, "dat\\language.ini");
    if (szKey1) strncpy(szBuf1, szKey1, sizeof(szBuf1)-1);
    if (szKey2) strncpy(szBuf2, szKey2, sizeof(szBuf2)-1);

    GetPrivateProfileString("LANGUAGE", "SELECTED", "ENGLISH", szLanguage,
        sizeof(szLanguage)-1, szDataFile);
    GetPrivateProfileString(szLanguage, "LANGUAGE-FILE", "language.en", szLangFile,
        sizeof(szLangFile)-1, szDataFile);
    GetPrivateProfileString(szLanguage, szName, "", szTempLine, MAX_PATH, szLangFile);
    if (!strlen(szTempLine)) {
        GetPrivateProfileString("ENGLISH", "LANGUAGE-FILE", "language.en", szLangFile,
            sizeof(szLangFile)-1, szDataFile);
        GetPrivateProfileString("ENGLISH", szName, szName, szTempLine, MAX_PATH, szLangFile);
    }

#ifdef _DEBUG
    if (!strlen(szLanguage) || !strlen(szTempLine)) {
        sprintf(szTempLine, "Failed to get string: %s(%s)", szName, szLanguage);
        MessageBox(0, szTempLine, szDataFile, MB_OK);
    }
#endif

    strcpy(szLine, "\0");
    ptr = strstr(szTempLine, "%1");
    if (ptr && szKey1) {
        ptr[0] = '\0';
        ptr += 2;
        sprintf(szBuf, "%s%s%s", szTempLine, szBuf1, ptr);
        strncpy(szTempLine, szBuf, sizeof(szTempLine)-1);
    }
    ptr = strstr(szTempLine, "%2");
    if (ptr && szKey2) {
        ptr[0] = '\0';
        ptr += 2;
        sprintf(szBuf, "%s%s%s", szTempLine, szBuf2, ptr);
        strncpy(szTempLine, szBuf, sizeof(szTempLine)-1);
    }
    if (strlen(szTempLine) >= 1024)
        strncpy(szLine, szTempLine, 1024-1);
    else
        strcpy(szLine, szTempLine);

    return szLine;
}

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    switch (dwReason) {

    case DLL_PROCESS_ATTACH:
        init_lock();
        break;
        
    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        release_lock();
        break;
    }

    return 1;
}
