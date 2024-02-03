#include "file_list_model.h"
#include "../lib/print.h"
#include "../lib/file_manage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FileList* get_file_list(char* addres){
    FILE *file = fopen(addres, "rb");
    fseek(file, 0, SEEK_END);
    int len = ftell(file)/sizeof(File);
    fseek(file, 0, SEEK_SET);
    FileList *flst = create_file_list(len);
    fread(flst->lst, sizeof(File), len, file);
    fclose(file);
    return flst;
}

void set_file_list(char* addres, FileList* flst){
    FILE* file = fopen(addres, "wb");
    fwrite(flst->lst, sizeof(File), flst->cnt, file);
    fclose(file);
    return;
}

void file_list_push_back(FileList* flst, File* fl){
    if(flst->cnt == flst->__cur_len){
        flst->__cur_len <<= 1;
        flst->lst = realloc(flst->lst, (flst->__cur_len)*sizeof(File));
    } 
    flst->lst[flst->cnt++] = *fl;
}

FileList* create_file_list(int num){
    FileList *flst = (FileList*)malloc(sizeof(FileList));
    flst->cnt = num;
    flst->__cur_len = (num<<1) + (num <= 0);
    flst->lst = (File*)malloc((flst->__cur_len)*sizeof(File));
    return flst;
}

void print_file_list(FileList *flst){ /* delete blanck file */
    flst = get_clean_file_list(flst);
    printf("count: %d\n", flst->cnt);
    for(int i = 0; i < flst->cnt; i++){
        print_file(flst->lst[i]);
    }
    print_warn("_________________");
}

void print_file_list_file(char* file_addres){
    FileList *flst = get_file_list(file_addres);
    print_file_list(flst);
}

State find_in_file_list(FileList *flst, char *query){
    for(int i = 0; i < flst->cnt; i++){
        if(!strcmp(flst->lst[i].addres, query)) return flst->lst[i].state;
    }
    return NotFound;
}

State find_in_file_list_with_addres(char* file_addres, char *query){
    FileList *flst = get_file_list(file_addres);
    return find_in_file_list(flst, query);
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
    FileList* flst = get_file_list(file_addres);
    return find_index_in_file_list(flst, query);
}

FileList* get_clean_file_list(FileList* flst){ /* Import!!!!! its change order of file!!!! */
    FileList *res = create_file_list(0);
    for(int i = 0; i < flst->cnt; i++){
        if(!is_blanck(&flst->lst[i])){
            file_list_push_back(res, &flst->lst[i]);
        }
    }
    return res;
}