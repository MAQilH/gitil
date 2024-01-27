#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* cat_string(char* a, char* b){
    char *nw = (char*)calloc(strlen(a) + strlen(b) + 2, 1);
    strcpy(nw, a);
    strcat(nw, b);
    return nw;
}