#include "config_model.h"
#include <stdio.h>

Config get_config(char* addres){
    Config conf;
    FILE *file = fopen(addres, "rb");
    fread(&conf, sizeof(conf), 1, file);
    fclose(file);
    return conf; 
}