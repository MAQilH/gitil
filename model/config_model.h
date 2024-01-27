#include "../glob.h"
typedef struct Config{
    char email[MAX_NAME];
    char name[MAX_NAME];
    char head[MAX_NAME];
    int date;
} Config;

Config get_config(char* addres);
