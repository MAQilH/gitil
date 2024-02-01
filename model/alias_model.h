#include "../glob.h"
typedef struct Alias{
    char from[MAX_MESSAGE];
    char to[MAX_MESSAGE];
} Alias;

void print_alias(Alias als);
void print_alias_file(char* file_addres);