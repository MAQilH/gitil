#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "headers.h"
#include "glob.h"
#include "model/string_list_model.h"
#include "lib/lib.h"

int main(int argc, char* argv_tmp[]){
    char **argv = argv_tmp;
    if(argc == 2){
        char* command = get_alias(argv[1]);
        if(command != NULL){
            StringList stl = get_string_list(command, " ");
            argv = stl.lst;
            argc = stl.cnt;
        }
    }
    char* act = argv[1];
    if(!strcmp(act, "init")) init(argc, argv);
    else if(!strcmp(act, "config")) config(argc, argv);
    return 0;
}