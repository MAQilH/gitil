#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <Windows.h>
#include <sys/stat.h>
#include <stdio.h>
#include "lib.h"
#include "../model/file_list_model.h"

void create_folder(char* addres){
    CreateDirectory(addres, NULL);
}

void create_file(char* name, char* addres){
    char *full_addres = cat_string(addres, cat_string("\\", name));
    FILE* file = fopen(full_addres, "wb");
    fclose(file);
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

int is_directory(char *path){
    DIR* dir = opendir(path);
    if (dir != NULL) {
        closedir(dir);
        return 1;
    }
    return 0;
}

int file_cmp(char* file_addres1, char* file_addres2){
    FILE* file1 = fopen(file_addres1, "r");
    FILE* file2 = fopen(file_addres2, "r");
    char c1, c2;
    while(1){
        c1 = fgetc(file1);
        c2 = fgetc(file2);
        if(c1 != c2){
            fclose(file1);
            fclose(file2);
            return 0;
        }
        if(c1 == EOF){
            fclose(file1);
            fclose(file2);
            return 1;
        }
    }
}

void merge_file_list(FileList *dest, FileList *src){
    for(int i = 0; i < src->cnt; i++){
        dest->lst[dest->cnt++] = src->lst[i];
    }
}