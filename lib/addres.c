#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include "lib.h"

char* get_current_addres(){
    char cwd[260];
    getcwd(cwd, sizeof(cwd));
    return get_string_ref(cwd);
}

char* get_root_addres(){
    char cwd[260];
    char *saved = get_current_addres();
    while(getcwd(cwd, sizeof(cwd)) != NULL && strcmp(cwd, "C:\\")){
        if(check_in_root()){
            chdir(saved);
            return get_string_ref(cwd);
        }
        chdir("..");
    }
    chdir(saved);
    return NULL;
}

char* get_local_config_addres(){
    return cat_string(get_root_addres(), "\\.gitil\\config_info.dat");
}

char* get_global_config_addres(){
    return "C:\\.gitil\\config_info.dat";
}

char* get_local_alias_addres(){
    return cat_string(get_root_addres(), "\\.gitil\\alias_info.dat");
}

char* get_global_alias_addres(){
    return "C:\\.gitil\\alias_info.dat";
}

char* get_branch_info_addres(){
    return cat_string(get_root_addres(), "\\.gitil\\branch_info.dat");
}