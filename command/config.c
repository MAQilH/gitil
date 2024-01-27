#include "../model/config_model.h"
#include "../model/alias_model.h"
#include "../lib/lib.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

void config(int argc, char *argv[]){

}

int create_config(char* addres){
    char* folder_addres = cat_string(addres, "\\.gitil");
    if(exist_folder(folder_addres)) return 0;
    create_hidden_folder(folder_addres);
    char* config_addres = cat_string(folder_addres, "\\config_info.dat");
    FILE* file = fopen(config_addres, "wb");
    Config empty;
    fwrite(&empty, sizeof(empty), 1, file);
    fclose(file);
    char* alias_addres = cat_string(folder_addres, "\\alias_info.dat");
    file = fopen(alias_addres, "wb");
    fclose(file);
    return 1;
}

void create_global_config(){
    create_config("C:\\");
}

void create_local_config(){
    create_config(get_curren_addres());
}