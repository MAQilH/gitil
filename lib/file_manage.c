#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
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

void merge_file_list(FileList *dest, FileList src){
    for(int i = 0; i < src.cnt; i++){
        dest->lst[dest->cnt++] = src.lst[i];
    }
}

void upate_middle_file(void* data, int size, int index, char* file_addres){
    FILE* file = fopen(file_addres, "rb+");
    fseek(file, index*size, SEEK_SET);
    fwrite(data, size, 1, file);
    fclose(file);
}

int remove_directory(char *path) {
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;
    if(d) {
        struct dirent *p;
        r = 0;
        while(!r && (p=readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
                continue;
            len = path_len + strlen(p->d_name) + 2; 
            buf = malloc(len);
            if (buf) {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);
                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode))
                        r2 = remove_directory(buf);
                    else
                        r2 = unlink(buf);
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }
    if(!r){
       r = rmdir(path);
    }
    return r;
}

int file_copy(char FileSource[], char FileDestination[]){
    int c;
    FILE *stream_R;
    FILE *stream_W; 

    stream_R = fopen (FileSource, "r");
    if(stream_R == NULL) return -1;
    stream_W = fopen (FileDestination, "w");
    if(stream_W == NULL){
        fclose(stream_R);
        return -2;
    }    
    while((c = fgetc(stream_R)) != EOF)
        fputc(c, stream_W);
    fclose(stream_R);
    fclose(stream_W);
    return 0;
}

void pop_from_file(int size, char* file_addres){
    int file = open(file_addres, O_RDWR);
    off_t sz = lseek(file, 0, SEEK_END);
    ftruncate(file, sz - size);
    close(file);
    return;
}

int check_exist_in_folder(char *addres, char *folder_addres){
    if(strlen(folder_addres) > strlen(addres)) return 0;
    return strncmp(addres, folder_addres, strlen(folder_addres)) == 0;
}