#include <stdio.h>
#include "summer.h"

int main(int argc, char* argv[]){
    printf("num: %d\n%s + %s\n", argc, argv[1], argv[2]);
    int a = 3, b = 82334;
    printf("%d", sum(a, b));
}
