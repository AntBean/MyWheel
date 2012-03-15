#ifndef ATTACK_H_INCLUDE
#define ATTACK_H_INCLUDE

#include "pluginscheduler.h"

#define MAX_THREADS     300

typedef int (CALLBACK *LOAD_PLUGIN_PROC)(int, int);
typedef int (CALLBACK *GET_IF_PROC)(char *);

typedef struct _thread_params {
    plugins_scheduler_t sched;
    struct scheduler_plugin *plugin;
    struct arglist *params;
    struct kb_item **kb;
    struct arglist *script_infos;
    char *script_name;
    int mode;
} thread_params;

typedef struct _thread_infos {
    char *script_name;
    HANDLE thread_handle;
    DWORD start_time;
    DWORD timeout;
    thread_params *thread_info;
} thread_infos;

int load_plugins(char *path, char *ext, struct arglist *plugins, struct arglist *prefs, LOAD_PLUGIN_PROC load_plugin_proc);
void free_plugins(struct arglist *plugins);
void set_timeout(int script_timeout, int network_timeout);
void set_work_dir(char *scripts, char *include);
void get_work_dir(char *scripts, char *include);
void plug_set_parameter(struct arglist *args, char *name, int type, void *value);
void *plug_get_parameter(struct arglist *args, char *name);
struct arglist *nasl_plugin_get_info(char *name, struct arglist *pref);
void init_lock();
void set_lock();
void unset_lock();
void release_lock();
void arg_to_file(struct arglist *, FILE *, char *);
int file_to_arg(FILE *, struct arglist *);
int check_host_summary(char *target, char *port_range, struct arglist *params);
int start_attack(struct arglist *params);

#endif