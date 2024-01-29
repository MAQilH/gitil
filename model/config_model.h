#include "../glob.h"
typedef struct Config{
    char email[MAX_EMAIL];
    char name[MAX_NAME];
    char head[MAX_NAME];
    char head_commit[MAX_NAME];
    int date;
} Config;

Config get_config(char* addres);
void print_config(Config cnf);
void print_config_file(char* file_addres);
