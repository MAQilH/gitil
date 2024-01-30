#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib.h"
#include "../model/string_list_model.h"

char* get_string(int len){
    return (char*)calloc(len + 1, sizeof(char));
}

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
        res.lst[res.cnt] = get_string(strlen(token));
        strcpy(res.lst[res.cnt], token);
        res.cnt++;
        token = strtok(NULL, del);
    }
    return res;
}

char* itos(int a){
    if(a == 0) return "0";
    char* str = get_string(MAX_COMA);
    int len = 0, tmp = a;
    while(tmp) len++, tmp /= 10;
    while(a){
        len--;
        str[len] = a%10 + '0';
        a /= 10;
    }
    return str;
}

int stoi(char* s){
    int res = 0, len = strlen(s);
    for(int i = 0; i < len; i++){
        res = res*10 + (s[i] - '0');
    }
    return res;
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

char* get_hash(char* s){
    int len = strlen(s);
    long long seed = 0;
    for(int i = 0; i < len; i++){
        seed = ((long long)seed*PRIME + s[i])%MOD;
    }
    srand(seed);
    char* hash = get_string(MAX_HASH);
    for(int i = 0; i < MAX_HASH; i++){
        hash[i] = (char)(rand()%26 + 'a');
    }
    return hash;
}

char* replace_name_with_hash(char* file_addres){
    return cat_string(
        cat_string(get_hash(file_addres), "."),
        get_file_type(file_addres) 
    );
}

int is_directory(char *path){
    StringList stl = get_string_list(path, "\\");
    char *last = stl.lst[stl.cnt-1];
    StringList stl2 = get_string_list(last, ".");
    return stl2.cnt < 2;
}

int is_file(char *path){
    return !is_directory(path);
}