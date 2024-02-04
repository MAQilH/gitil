#include "../lib/lib.h"
#include "add.h"
#include "stage.h"
#include "status.h"
#include "../model/string_list_model.h"
#include "../model/file_list_model.h"

int add_file_rel_addres_to_file_list(FileList *flst, char* addres){
    char *folder_addres = get_folder_addres(addres);
    FileList* tmp = create_file_list(0); 
    get_file_status(tmp, folder_addres, 1);
    int have = 0;
    for(int i = 0; i < tmp->cnt; i++){
        if(wildcard_checker(addres, tmp->lst[i].addres)){
            have = 1;
            file_list_push_back(flst, &tmp->lst[i]);
        }
    }
    return have;
}

void add_n(int dep){
    FileList *flst = create_file_list(0);
    get_file_status(flst, get_current_addres(), dep);
    for(int i = 0; i < flst->cnt; i++){
        if(is_blanck(&flst->lst[i])) continue;
        if(!in_stage(flst->lst[i].addres) || changed_file_stage_area(flst->lst[i].addres)){
            char *msg = strcat(get_rel_addres(flst->lst[i].addres), " not in stage!");
            print_warn(msg);
        } else{
            char *msg = strcat(get_rel_addres(flst->lst[i].addres), " in stage!");
            print_success(msg);
        }
    }
}

int add(int argc, char *argv[]){
    if(strcmp(get_current_commit(), get_cuurent_HEAD_commit())){
        print_fail("fail: you must be in head commit for add!");
        return 0;
    }
    if(argc > 2 && !strcmp(argv[2], "-redo")){
        redo();
        return 1;
    }
    if(argc > 2 && !strcmp(argv[2], "-n")){
        add_n(argc == 3? 1: stoi(argv[3]));
        return 1;
    }
    int ptr = 2;
    if(!strcmp(argv[ptr], "-f")){
        ptr++;
    }
    FileList *flst = create_file_list(0);
    for(; ptr < argc; ptr++){
        int flg_file = 0;
        if(*argv[ptr] == '^'){
            argv[ptr]++;
            flg_file = 1;
            argv[ptr][strlen(argv[ptr])-1] = '\0';
        }
        char* addres = get_current_addres();
        addres = cat_string(addres, argv[ptr]);
        if(!flg_file && is_directory(addres)){
            get_file_status(flst, addres, MAX_DEP);
        } else if(flg_file || is_file(addres)){            
            if(!add_file_rel_addres_to_file_list(flst, addres)){
                char *msg = "warn: no file/folder ";
                msg = cat_string(msg, argv[ptr]);
                msg = cat_string(msg, " exist!");
                print_warn(msg);
            }
        }
    }
    printf("%d files added.\n", flst->cnt);
    add_to_stage(flst);
    return 1;
}

void add_all_changes(){
    FileList *flst = create_file_list(0);
    get_file_status(flst, get_root_addres(), MAX_DEP);
    add_to_stage(flst);
}