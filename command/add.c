#include "../lib/lib.h"
#include "add.h"
#include "stage.h"
#include "../model/string_list_model.h"
#include "../model/file_list_model.h"

int add_file_rel_addres_to_file_list(FileList *flst, char* addres){
    File fl = {.state = get_file_state_with_commit(get_cuurent_HEAD_commit(), addres)};
    if(fl.state == NotFound) return 0;
    strcpy(fl.addres, addres);
    strcpy(fl.name, get_file_name(fl.addres));
    flst->lst[flst->cnt++] = fl;
    return 1;
}

void add_n(int dep){
    FileList flst = {.cnt = 0};
    get_file_status(&flst, get_current_addres(), dep);
    for(int i = 0; i < flst.cnt; i++){
        if(in_stage(flst.lst[i].addres)){
            char *msg = strcat(get_rel_addres(flst.lst[i].addres), " in stage!");
            print_success(msg);
        } else{
            char *msg = strcat(get_rel_addres(flst.lst[i].addres), " not in stage!");
            print_warn(msg);
        }
    }
}

void add(int argc, char *argv[]){
    if(argc > 2 && !strcmp(argv[2], "-redo")){
        redo();
        return;
    }
    if(argc > 2 && !strcmp(argv[2], "-n")){
        add_n(argc == 3? 1: stoi(argv[3]));
        return;
    }
    int ptr = 2;
    if(!strcmp(argv[ptr], "-f")){
        ptr++;
    }
    FileList flst = {.cnt = 0};
    for(; ptr < argc; ptr++){
        char* msg = cat_string(argv[ptr], " has been successfully added to the stage!");
        char* addres = get_current_addres();
        addres = cat_string(addres, argv[ptr]);
        if(is_directory(addres)){
            get_file_status(&flst, addres, MAX_DEP);
            print_success(msg);
        } else if(is_file(addres)){            
            if(add_file_rel_addres_to_file_list(&flst, addres)){
                print_success(msg);
            } else{
                char *msg = "warn: no file/folder ";
                msg = cat_string(msg, argv[ptr]);
                msg = cat_string(msg, " exist!");
                print_warn(msg);
            }
        }
        add_to_stage(&flst);
    }
}