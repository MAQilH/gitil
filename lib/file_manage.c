#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <Windows.h>
#include <sys/stat.h>
#include "lib.h"

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

int exist_folder(char* addres){
    struct stat sb;
    return stat(addres, &sb) == 0 && S_ISDIR(sb.st_mode);
}

int exist_file(char* addres){
    return access(addres, F_OK) != -1;
}