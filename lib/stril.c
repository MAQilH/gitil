#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

char* get_string_ref(char *c){
    char *tmp = (char*)calloc(strlen(c) + 1, 1);
    strcpy(tmp, c);
    return tmp;
}

char* cat_string(char* a, char* b){
    char *nw = (char*)calloc(strlen(a) + strlen(b) + 2, 1);
    strcpy(nw, a);
    strcat(nw, b);
    return nw;
}