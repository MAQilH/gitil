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

int a = 2, b = 4;

void f(int* x){
    x = &a;
}

int main(){
    char* a = (char*)calloc(2, 1);
    strcat(a, "afsdfsdf");
    printf("%s\n", a);
    return 0;
}