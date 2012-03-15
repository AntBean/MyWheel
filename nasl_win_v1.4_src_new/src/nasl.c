/* Nessus Attack Scripting Language 
 *
 * Copyright (C) 2002 - 2005 Tenable Network Security
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2,
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 */
#include "includes.h"
#include "libnessus.h"

#include "nasl.h"
#include "attack.h"
#include "nasl_tree.h"
#include "nasl_global_ctxt.h"
#include "nasl_func.h"
#include "nasl_var.h"
#include "nasl_lex_ctxt.h"
#include "exec.h"
#include "tcp.h"
#include "list.h"
#include "report/report.h"

#include "getopt.h"

#ifndef MAP_FAILED
#define MAP_FAILED ((void*)-1)
#endif


extern char * nasl_version();
extern int execute_instruction(struct arglist *, char *);
void exit_nasl(struct arglist *, int);

extern CRITICAL_SECTION criLock;

extern int safe_checks_only;
extern int check_svc_status;
extern char log_file[MAX_PATH];

void usage()
{
    printf("nasl for win v%s - replanted from nessus-2.2.4 by glacier@xfocus.org\n\n", nasl_version());
    printf("Usage: nasl [options] script_file ...\n");
    printf("[options] means:\n");
    printf("    -t <target>: execute the scripts against the target host\n");
    printf("    -d <directory>: specifies the work directory\n");
    printf("    -m <number>: specifies the maximal threads number\n");
    printf("    -s: specifies that the script should be run with 'safe checks' enabled\n");
    printf("    -i <interface_number>: specifies the network interface\n");
    printf("    -l <file>: specifies the log file\n");
    printf("\n");
    printf("    -v: display verbose information\n");
    printf("    -T <file>: trace actions into the file (or '-' for stderr)\n");
    printf("    -M <mode>: special mode of scripts execution\n");
    printf("        P: parse only - do not execute the script\n");
    printf("        D: run the 'description part' only\n");
    printf("        L: 'lint' the script (extended checks)\n");
    printf("        S<filename>: sign the script\n");
    printf("        X: Run the script in 'authenticated' mode\n");
    printf("    -L: list network interface\n");
    printf("    -h: shows this help screen\n");
    printf("Example:\n");
    printf("    nasl -L\n");
    printf("    nasl -t 192.168.0.1 -d scripts\n");
    printf("    nasl -t 192.168.0.1 -d scripts -i 2 -m 20 -l log.txt\n");
    printf("    nasl -t localhost -d scripts -s -v -l log.htm\n");
}

extern FILE    *nasl_trace_fp;

VOID PutAlert(struct arglist *desc, char *szLine)
{
}

int CALLBACK load_plugin_proc(int i, int n)
{
#ifndef _XSCAN_PLUG
    fprintf(stdout, "Loading scripts %d%% ...\r", i*100/n);
#endif
    return 1;
}

int main(int argc, char ** argv)
{
    struct arglist *plugins = NULL;
    int if_num = 0;
    char *target = NULL, *scripts_dir = NULL, *port_range = NULL, *scan_mode = NULL;
    char *kb_fname;
    int start, n, i, verbose = 0;
    char trace_file[MAX_PATH] = {0}, log_file[MAX_PATH] = {0};
    FILE *fp_log = NULL, *test = NULL;
    int mode = 0;

    int plugin_num = 0;
    char szPath[MAX_PATH] = {0}, szLine[MAX_PATH] = {0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {80, 9999};
    SYSTEMTIME StartTime, EndTime;
    struct arglist *key = NULL, *plugin = NULL, *prefs = NULL;
    struct arglist *templist = NULL, *filelist = NULL;
    char *ptr = NULL;
    struct arglist *params;

    int nMaxThread = 50, nCurrThread = 0, nPluginThread = 0;

    // Command-line options

    if(argc < 2) { 
        usage();
        return 0;
    }
 
    while((i = getopt(argc, argv, "hvLt:d:i:k:T:l:sSp:m:M:"))!=EOF)
    switch(i) {
    case 't' :
        if(optarg)target = strdup(optarg);
        else {
            usage();
            exit(1);
        }
        break;
    case 'd' :
        if(optarg)scripts_dir = strdup(optarg);
        else {
            usage();
            exit(1);
        }
        break;
    case 'i' :
        if(optarg) if_num = atoi(optarg);
        else {
            usage();
            exit(1);
        }
        break;
    case 'L' :
        iflist();
        return 0;
        break;
    case 'M' : {
	    if ( optarg == NULL ) {
		    usage();
		    exit(1);
		}
        switch (optarg[0]) {
        case 'P':
            mode |= NASL_EXEC_PARSE_ONLY;
            break;
        case 'D':
            mode |= NASL_EXEC_DESCR;
            break;
        case 'L':
            mode |= NASL_LINT;
            break;
        case 'S':
	        if ( optarg[1] == '\0' ) {
		        usage();
		        exit(1);
		    }
#ifdef HAVE_SSL
			 nessus_SSL_init(NULL);
	        generate_signed_script(optarg+1);
#endif
	        exit(0);
        case 'X':
            mode |= NASL_ALWAYS_SIGNED;
            break;
        }
        break;
    }
    case 'T':
        strcpy(trace_file, optarg);
        break;
    case 'l':
        if (strchr(optarg, ':'))
            strcpy(log_file, optarg);
        else {
            GetModuleFileName(0, log_file, sizeof(log_file)-1);
            ptr = strrchr(log_file, '\\');
            if (ptr) ptr[0] = '\0';
            strcat(log_file, "\\"); strcat(log_file, optarg);
        }
        break;
    case 'h' :
        usage();
        exit(0);
        break;
    case 'k':
        if(optarg)kb_fname = strdup(optarg);
        else {
            usage();
            exit(0);
        }
        break;
    case 'v' :
        verbose = 1;
        break;
    case 's' :
        safe_checks_only++;
        break;
    case 'p' :
        if(optarg){
            port_range = strdup(optarg);
            scan_mode = strrchr(port_range, ':');
            if (scan_mode) {
                scan_mode[0] = '\0'; scan_mode++;
            }
        }
        else {
            usage();
            exit(0);
        }
        break;
    case 'm':
        if(optarg) nMaxThread = atoi(optarg);
        else {
            usage();
            exit(1);
        }
        break;
    default:
        usage();
        exit(0);
    }
 
    // init global var

    if (!target) target = strdup("127.0.0.1");

    start = optind;
    n = start;

    text_color(7);
    SetConsoleScreenBufferSize(console, coord);
#ifndef _DEBUG
    if (!verbose) CloseHandle(GetStdHandle(STD_ERROR_HANDLE));
#endif
    GetModuleFileName(0, szPath, sizeof(szPath)-1);
    ptr = strrchr(szPath, '\\');
    if (ptr) ptr[0] = '\0';
    SetCurrentDirectory(szPath);
    if (scripts_dir) {
        sprintf(szLine, "%s\\include", scripts_dir);
        set_work_dir(scripts_dir, szLine);
    }

    if (!strlen(log_file)) sprintf(log_file, "%s\\%s.htm", szPath, target);
    fp_log = fopen(log_file, "w");
    if (fp_log == NULL) {
        perror(log_file);
        return 0;
    }
    if (fp_log) fclose(fp_log);

    setif(if_num);
    tcp_init();

    params = (struct arglist *)emalloc(sizeof(struct arglist));

    plug_set_parameter(params, "HostName", ARG_STRING, target);
    plug_set_parameter(params, "ScriptsDir", ARG_STRING, scripts_dir);
    sprintf(szLine, "%s\\include", scripts_dir);
    plug_set_parameter(params, "IncludeDir", ARG_STRING, szLine);
    plug_set_parameter(params, "Port/Range", ARG_STRING, port_range);
    plug_set_parameter(params, "Port/ScanMode", ARG_STRING, scan_mode);
    plug_set_parameter(params, "Port/IdentifyService", ARG_INT, 1);
    plug_set_parameter(params, "Port/OSAffirmed", ARG_INT, 1);
    plug_set_parameter(params, "ExecMode", ARG_INT, mode);
    plug_set_parameter(params, "MaxThreads", ARG_INT, nMaxThread);
    plug_set_parameter(params, "CurrentThreads", ARG_PTR, &nCurrThread);
    plug_set_parameter(params, "PluginThreads", ARG_PTR, &nPluginThread);
    plug_set_parameter(params, "TraceFile", ARG_STRING, trace_file);
    plug_set_parameter(params, "LogFile", ARG_STRING, log_file);
    plug_set_parameter(params, "ShowVerbose", ARG_INT, verbose);

    GetLocalTime(&StartTime);

    // check if the target host is active
    if (!(mode & NASL_EXEC_PARSE_ONLY) && port_range) {
        check_host_summary(target, port_range, params);
    }
    if (key && (int)arg_get_value(key, "Host/dead") > 0) {
        printf("Target host is dead.\n");
        arg_free_all(params);
        tcp_exit();
        return 0;
    }

    // load scripts from param
    printf("\n");
    get_work_dir(szLine, NULL);
    store_init_sys(szLine);

    plugins = emalloc(sizeof(struct arglist));
    prefs = arg_get_value(params, "preferences");
    if (!prefs) {
        prefs = emalloc(sizeof(struct arglist));
        arg_add_value(params, "preferences", ARG_PTR, sizeof(struct arglist), prefs);
    }

    if (argv[optind]) {
        n = start; plugin_num = 0;
        filelist = emalloc(sizeof(struct arglist));
        while(argv[n]) {
            arg_add_value(filelist, argv[n], ARG_INT, sizeof(int), (void*)1);
            plugin_num++;
            n++;
        }
        templist = filelist;
        printf("Loading plug-ins ...");
        while (templist && templist->name) {
            nasl_plugin_add(templist->name, plugins, prefs, 1);
            printf(".");
            templist = templist->next;
        }
        printf(" completed\n\n");
        if (filelist) arg_free_all(filelist);
    }
    else {
        // load scripts from directory
        get_work_dir(szLine, NULL);
        plugin_num = load_plugins(szLine, "*.nasl", plugins, prefs, load_plugin_proc);
    }
#if 0
    printf("plug-ins list:\n");
    arg_dump(plugins, 0);
    printf("\n");
#endif

    plug_set_parameter(params, "ScriptList", ARG_PTR, plugins);
    plug_set_parameter(params, "ScriptNumber", ARG_INT, plugin_num);
    plug_set_parameter(params, "CheckDependence", ARG_INT, 1);
    set_timeout(180, 5);

    start_attack(params);

    GetLocalTime(&EndTime);

    if (strlen(log_file)) {
        int log_type = SAVE_TEXT;
        struct arglist *report = log_to_arglist(log_file);
        ptr = strrchr(log_file, '.');
        if (ptr) {
            ptr++;
            if (!stricmp(ptr, "txt"))
                log_type = SAVE_TEXT;
            else if (!stricmp(ptr, "htm") || !stricmp(ptr, "html"))
                log_type = SAVE_HTML;
            else if (!stricmp(ptr, "xml"))
                log_type = SAVE_XML;
            else
                log_type = SAVE_TEXT;
        }
        save_report(log_file, log_type, report);
        efree(&report);
    }

    if (plugins) {
        free_plugins(plugins);
#ifdef _DEBUG
        printf("free_plugins(plugins) complete.\n");
#endif
    }
    if (params) {
        arg_free_all(params);
#ifdef _DEBUG
        printf("arg_free_all(params) complete.\n");
#endif
    }
    tcp_exit();

    printf("\nAll scripts completed, %d/%d/%d/%d:%d:%d - %d/%d/%d/%d:%d:%d.\n",
        StartTime.wYear, StartTime.wMonth, StartTime.wDay, StartTime.wHour, StartTime.wMinute, StartTime.wSecond,
        EndTime.wYear, EndTime.wMonth, EndTime.wDay, EndTime.wHour, EndTime.wMinute, EndTime.wSecond);

#ifdef _DEBUG
    printf("\nPress any key to exit.\n");
    getch();
#endif

    return(0);
}
