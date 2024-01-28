#include "config_model.h"
#include <stdio.h>
#include "../lib/stril.h"

Config get_config(char* addres){
    Config conf;
    FILE *file = fopen(addres, "rb");
    fread(&conf, sizeof(conf), 1, file);
    fclose(file);
    return conf; 
}

void print_config(Config cnf){
    printf(
        "name: %s\n"
        "email: %s\n"
        "HEAD: %s\n"
        "date: %s\n\n",
        cnf.name, cnf.email, cnf.head, get_date(cnf.date)
    );
}

void print_config_file(char* file_addres){
    FILE* file = fopen(file_addres, "rb");
    Config tmp;
    while(fread(&tmp, sizeof(tmp), 1, file)){
        print_config(tmp);
    }
    fclose(file);
}