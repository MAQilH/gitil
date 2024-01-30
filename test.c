#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
// #include "model/model.h"

#define MAX_NAME (260) 

int check_exist_in_folder(char *addres, char *folder_addres){
    if(strlen(folder_addres) > strlen(addres)) return 0;
    return strncmp(addres, folder_addres, strlen(folder_addres)) == 0;
}

void f(char* s){
    printf("%s\n", s);
}

int main(){
    f("sadffs");
    return 0;
}