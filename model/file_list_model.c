#include "file_list_model.h"
#include "../lib/print.h"
#include <stdio.h>
#include <string.h>

FileList get_file_list(char* addres){
    FILE *file = fopen(addres, "rb");
    FileList flst;
    fread(&flst, sizeof(flst), 1, file);
    fclose(file);
    return flst;
}

void set_file_list(char* addres, FileList* flst){
    FILE* file = fopen(addres, "wb");
    fwrite(flst, sizeof(*flst), 1, file);
    fclose(file);
    return;
}

void print_file_list(FileList flst){ /* delete blanck file */
    flst = get_clean_file_list(&flst);
    printf("count: %d\n", flst.cnt);
    for(int i = 0; i < flst.cnt; i++){
        print_file(flst.lst[i]);
    }
    print_warn("_________________");
}

void print_file_list_file(char* file_addres){
    FILE* file = fopen(file_addres, "rb");
    FileList tmp;
    while(fread(&tmp, sizeof(tmp), 1, file)){
        print_file_list(tmp);
    }
    fclose(file);
}

State find_in_file_list(FileList *flst, char *query){
    for(int i = 0; i < flst->cnt; i++){
        if(!strcmp(flst->lst[i].addres, query)) return flst->lst[i].state;
    }
    return NotFound;
}

State find_in_file_list_with_addres(char* file_addres, char *query){
    State res = NotFound;
    FILE* file = fopen(file_addres, "rb");
    FileList tmp;
    while(fread(&tmp, sizeof(tmp), 1, file)){
        State cr = find_in_file_list(&tmp, query);
        if(cr != NotFound) res = cr;
    }
    fclose(file);
    return res;
}

int find_index_in_file_list(FileList *flst, char* query){
    for(int i = 0; i < flst->cnt; i++){
        if(!strcmp(flst->lst[i].addres, query)){
            return i;
        }
    }
    return -1;
}

int find_index_in_file_list_with_addres(char *file_addres, char* query){
    FILE* file = fopen(file_addres, "rb");
    FileList flst;
    fread(&flst, sizeof(flst), 1, file);
    fclose(file);
    return find_index_in_file_list(&flst, query);
}

FileList get_clean_file_list(FileList* flst){ /* Import!!!!! its change order of file!!!! */
    FileList res = {.cnt = 0};
    for(int i = 0; i < flst->cnt; i++){
        if(!is_blanck(&flst->lst[i])){
            res.lst[res.cnt++] = flst->lst[i];
        }
    }
    return res;
}