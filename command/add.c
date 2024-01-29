#include "../lib/lib.h"
#include "add.h"
#include "stage.h"
#include "../model/string_list_model.h"
#include "../model/file_list_model.h"

void add_file_rel_addres_to_file_list(FileList *flst, char* rel_addres){
    char* addres = get_current_addres();
    addres = cat_string(addres, rel_addres);
    File fl = {.state = get_file_state_with_commit(get_cuurent_HEAD_commit(), addres)};
    strcpy(fl.addres, addres);
    strcpy(fl.name, get_file_name(fl.addres));
    flst->lst[flst->cnt++] = fl;
    return;
}

void add_folder_addres_to_file_list(FileList *flst, char* rel_addres){
    char *saved = get_current_addres();
    chdir(rel_addres);
    get_file_status(flst, get_current_addres());
    chdir(saved);
    return;
}

void add(int argc, char *argv[]){
    if(!strcmp(argv[2], "-redo")){
        redo_from_stage();
        return;
    }
    int ptr = 2;
    if(!strcmp(argv[ptr], "-f")){
        ptr++;
    }
    FileList flst = {.cnt = 0};
    for(; ptr < argc; ptr++){
        char* msg = cat_string(argv[ptr], " has been successfully added to the stage!");
        if(is_directory(argv[ptr])){
            add_folder_addres_to_file_list(&flst, argv[ptr]);
            print_success(msg);
        } else if(exist_file(argv[ptr])){
            add_file_rel_addres_to_file_list(&flst, argv[ptr]);
            print_success(msg);
        } else{
            char *msg = "warn: no file/folder ";
            msg = cat_string(msg, argv[ptr]);
            msg = cat_string(msg, " exist!");
            print_warn(msg);
        }
        add_to_stage(flst);
    }
}