#include <stdio.h>
#include <string.h>
#include "headers.h"



int main(int argc, char* argv[]){
    char* act = argv[1];
    if(!strcmp(act, "init")) init(argc, argv);
    return 0;
}