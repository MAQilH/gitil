#include <stdio.h>
#include <string.h>
#include "print.h"

void prints(char* s){
    printf("%s\n", s);
}
void printsd(char* s){
    printf("debug: %s\n", s);
}
void print_fail(char* s){
    printf("\033[1;31m");
    printf(s);
    printf("\033[0m\n");
}
void print_success(char *s){
    printf("\033[1;32m");
    printf(s);
    printf("\033[0m\n");
}
void print_warn(char *s){
    printf("\033[1;33m");
    printf(s);
    printf("\033[0m\n");
}