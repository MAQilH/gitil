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
    res.lst = (char**)calloc(MAX_ADDRES, sizeof(char*));
    while(token != NULL){
        res.lst[res.cnt] = (char*)calloc(strlen(token) + 1, sizeof(char));
        strcpy(res.lst[res.cnt], token);
        res.cnt++;
        token = strtok(NULL, del);
    }
    return res;
}

char* itos(int a){
    if(a == 0) return "0";
    char* str = (char*)calloc(MAX_NAME, 1);
    int len = 0, tmp = a;
    while(tmp) len++, tmp /= 10;
    while(a){
        len--;
        str[len] = a%10 + '0';
        a /= 10;
    }
    return str;
}

char* get_file_name(char* file_addres){
    StringList stl = get_string_list(file_addres, "\\");
    return get_string_ref(stl.lst[stl.cnt-1]);
}

char* get_file_type(char* file_addres){
    StringList stl = get_string_list(file_addres, ".");
    return get_string_ref(stl.lst[stl.cnt-1]);
}

char* get_date(int date){
    time_t epoch_seconds = date;
    struct tm *tm_info = localtime(&epoch_seconds);
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    return get_string_ref(buffer);
}


char* replace_name_with_id(char* file_addres, int index){
    return cat_string(
        cat_string(itos(index), "."),
        get_file_type(file_addres) 
    );
}