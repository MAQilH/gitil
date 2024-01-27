#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "../lib/lib.h"
#include "config.h"

void initialize(){
    create_local_config();
    create_global_config();
    chdir("./.gitil");
    FILE *file = fopen("barnchs_info.dat", "wb");
    fclose(file);
    create_folder(cat_string(get_curren_addres(), "\\branch"));
}

int init(int argc, char *argv[]){
    if(get_root_project_addres() != NULL){
        print_fail("fail: gitil already created!");
        return 0;
    }
    initialize();
    print_success("gitil created!");
    return 1;
}