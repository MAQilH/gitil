#include <stdio.h>
#include "alias_model.h"

void print_alias(Alias als){
    printf("%s -> %s\n", als.from, als.to);
}

void print_alias_file(char* file_addres){
    FILE* file = fopen(file_addres, "rb");
    Alias tmp;
    while(fread(&tmp, sizeof(tmp), 1, file)){
        print_alias(tmp);
    }
    fclose(file);
}