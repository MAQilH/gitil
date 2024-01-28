#include "file_list_model.h"
#include "../lib/print.h"
#include <stdio.h>

void print_file_list(FileList flst){
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