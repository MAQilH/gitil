#include "../model/config_model.h"
#include "../model/alias_model.h"
#include "../lib/lib.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

void set_username(char* addres, char* user_name){
    printsd(addres);
    printsd(user_name);
    FILE* file = fopen(addres, "rb");
    Config conf;
    fread(&conf, sizeof(conf), 1, file);
    fclose(file);
    strcpy(conf.name, user_name);
    file = fopen(addres, "wb");
    fwrite(&conf, sizeof(conf), 1, file);
    fclose(file);
}

void set_email(char* addres, char* email){
    FILE* file = fopen(addres, "rb");
    Config conf;
    fread(&conf, sizeof(conf), 1, file);
    fclose(file);
    strcpy(conf.email, email);
    file = fopen(addres, "wb");
    fwrite(&conf, sizeof(conf), 1, file);
    fclose(file);
}

void add_alias(char* addres, Alias als){
    FILE* file = fopen(addres, "ab");
    fwrite(&als, sizeof(als), 1, file);
    fclose(file);
}

void config(int argc, char *argv[]){
    int in_global = 0;
    for(int i = 0; i < argc; i++){
        printsd(argv[i]);
    }
    if(!strcmp(argv[2],"-global")) in_global = 1;
    char* type = argv[in_global + 2];
    if(!strcmp(type, "user.name")){
        set_username(in_global? get_global_config_addres(): get_local_config_addres(), argv[argc-1]);
    } else if(!strcmp(type, "user.email")){
        set_email(in_global? get_global_config_addres(): get_local_config_addres(), argv[argc-1]);
    } else{ // alias
        Alias als;
        strcpy(als.from, argv[argc-2]);
        strcpy(als.to, argv[argc-1]);
        add_alias(in_global? get_global_alias_addres(): get_local_alias_addres(), als);
    }
    FILE *file = fopen(get_local_alias_addres(), "rb");
    Alias als;
    while(fread(&als, sizeof(als), 1, file)){
        printf("%s %s\n", als.from, als.to);
    }
    fclose(file);
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
    create_config(get_current_addres());
}
