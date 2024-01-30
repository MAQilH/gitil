#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "headers.h"
#include "glob.h"
#include "lib/lib.h"
#include "model/model.h"

void test_env(int argc, char *argv[]){
    // printf("asd\n");
    print_config_file(get_global_config_addres());
    // print_file_list_file(get_current_stage_info_addres());
    printf("%d", file_cmp(get_local_config_addres(), get_local_config_addres()));
}

int check_continue(int argc, char *argv[]){
    char* act = argv[1];
    return !strcmp(act, "init") || (!strcmp(act, "config") && !strcmp(argv[2], "-global")) || get_root_addres() != NULL;
}

int main(int argc, char* argv_tmp[]){
    if(argc == 1 || !strcmp(argv_tmp[1], "test")){
        test_env(argc, argv_tmp);
        return 0;
    }

    char **argv = argv_tmp;
    if(argc == 2){
        char* command = get_alias(argv[1]);
        if(command != NULL){
            StringList stl = get_string_list(command, " ");
            argv = stl.lst;
            argc = stl.cnt;
        }
    }
    if(!check_continue(argc, argv)){
        print_fail("fail: gitil not created!");
        return 0;
    }

    char* act = argv[1];
    if(!strcmp(act, "init")) init(argc, argv);
    else if(!strcmp(act, "config")) {
        if(config(argc, argv)){
            print_success("config changes!");
        }
    } else if(!strcmp(act, "delete")){
        del(argc, argv);
        print_warn("fuck you :)");
    } else if(!strcmp(act, "add")){
        add(argc, argv);
    } else if(!strcmp(act, "reset")){
        reset(argc, argv);
    } else if(!strcmp(act, "status")){
        status(argc, argv);
    } else if(!strcmp(act, "commit")){
        commit(argc, argv);
    }
    else {
        print_fail("fail: input is invalid!");
    }


    return 0;
}