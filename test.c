#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
// #include "model/model.h"

#define MAX_NAME (260) 

int* f(int *a){
    a = realloc(a, 10000*sizeof(int));
    a[0] = 23;
    printf("%p %d\n", a, *a);
    return a;
}

int main(){
    int* a = (int*)calloc(1, sizeof(int));
    a[0] = 54;
    printf("%p %d\n", a, *a);
    a = f(a);
    printf("%p %d\n", a, *a);
    return 0;
}