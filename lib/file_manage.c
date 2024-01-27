#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <Windows.h>
#include "print.h"

void create_folder(char* addres){
    CreateDirectory(addres, NULL);
}

void create_hidden_folder(char* addres){
    create_folder(addres);
    SetFileAttributes(addres, FILE_ATTRIBUTE_HIDDEN);
}

int check_in_root(){
    DIR *dir = opendir(".");
    struct dirent *ent;
    while((ent = readdir(dir)) != NULL){
        if(!strcmp(ent->d_name, ".gitil")){
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

char* get_ref(char *c){
    char *tmp = (char*)calloc(strlen(c) + 1, 1);
    strcpy(tmp, c);
    return tmp;
}

char* get_curren_addres(){
    char cwd[260];
    getcwd(cwd, sizeof(cwd));
    return get_ref(cwd);
}

char* get_root_project_addres(){
    char cwd[260];
    char *saved = get_curren_addres();
    while(getcwd(cwd, sizeof(cwd)) != NULL && strcmp(cwd, "C:\\")){
        if(check_in_root()){
            chdir(saved);
            return get_ref(cwd);
        }
        chdir("..");
    }
    chdir(saved);
    return NULL;
}
