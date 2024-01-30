#include "../glob.h"
typedef struct Config{
    char email[MAX_EMAIL];
    char name[MAX_NAME];
    char head[MAX_NAME];
    char current_commit[MAX_HASH];
    int date;
    int seed;
} Config;

Config get_config(char* addres);
void print_config(Config cnf);
void print_config_file(char* file_addres);
void set_config(char* addres, Config* conf);