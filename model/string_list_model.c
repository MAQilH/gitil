#include "string_list_model.h"
#include <stdio.h>
#include "../lib/print.h"


void print_string_list(StringList stl){
    print_warn("____\n");
    printf("count: %d\n", stl.cnt);
    for(int i = 0; i < stl.cnt; i++){
        printf("%s\n", stl.lst[i]);
    }
    print_warn("____\n");
}

void print_string_list_file(char* file_addres){
    FILE* file = fopen(file_addres, "rb");
    StringList tmp;
    while(fread(&tmp, sizeof(tmp), 1, file)){
        print_string_list(tmp);
    }
    fclose(file);
}