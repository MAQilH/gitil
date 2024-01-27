#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib.h"
#include "../model/string_list_model.h"

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

StringList get_string_list(char* str, char* del){
    char str_arr[strlen(str) + 1];
    strcpy(str_arr, str);
    char *token = strtok(str_arr, del);
    StringList res;
    res.cnt = 0;
    res.lst = (char**)calloc(MAX_NAME, sizeof(char*));
    while(token != NULL){
        res.lst[res.cnt] = (char*)calloc(strlen(token) + 1, sizeof(char));
        strcpy(res.lst[res.cnt], token);
        res.cnt++;
        token = strtok(NULL, del);
    }
    return res;
}