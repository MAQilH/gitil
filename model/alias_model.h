#include "../glob.h"
typedef struct Alias{
    char from[MAX_NAME];
    char to[MAX_NAME];
} Alias;

void print_alias(Alias als);
void print_alias_file(char* file_addres);