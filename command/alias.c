#include <stdio.h>
#include "../lib/lib.h"
#include "../model/alias_model.h"
#include "alias.h"
#include <string.h>

char* get_alias(char *arc){
    FILE *file;
    Alias als;
    if(get_root_addres() != NULL){
        file = fopen(get_local_alias_addres(), "rb");
        while(fread(&als, sizeof(als), 1, file)){
            if(!strcmp(als.from, arc)){
                fclose(file);
                return get_string_ref(als.to);
            }
        }
        fclose(file);
    }
    if(exist_folder(get_global_folder_addres())){
        file = fopen(get_global_alias_addres(), "rb");
        while(fread(&als, sizeof(als), 1, file)){
            if(!strcmp(als.from, arc)){
                fclose(file);
                return get_string_ref(als.to);
            }
        }
        fclose(file);
    }
    return NULL;
}