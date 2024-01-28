#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "config.h"
#include "../lib/lib.h"
#include "branch.h"

void initialize(){
    create_local_config();
    create_global_config();
    chdir("./.gitil");
    FILE *file = fopen("barnchs_info.dat", "wb");
    fclose(file);
    create_folder(get_main_branch_folder_addres());
    create_branch("master", "", 0);
}

int init(int argc, char *argv[]){
    if(get_root_addres() != NULL){
        print_fail("fail: gitil already created!");
        return 0;
    }
    initialize();
    print_success("gitil created!");
    return 1;
}