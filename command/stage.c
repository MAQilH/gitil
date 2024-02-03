#include "../lib/lib.h"
#include "../model/file_list_model.h"

char* get_name_file_in_stage_change(char* file_addres){
    return replace_name_with_hash(file_addres);
}

int in_stage(char *file_addres){
    return find_in_file_list_with_addres(
        get_current_stage_info_addres(),
        file_addres
    ) != NotFound;
}

void add_to_mem_stage(FileList* flst){
    FileList *mem_stage = get_file_list(get_current_mem_stage_info_addres());
    for(int i = 0; i < flst->cnt; i++){
        file_list_push_back(mem_stage, &flst->lst[i]);
    }
    set_file_list(get_current_mem_stage_info_addres(), mem_stage);
}

void redo(){
    FileList *mem_stage = get_file_list(get_current_mem_stage_info_addres());

    FileList *add_file_list = create_file_list(0);
    for(int i = 0; i < mem_stage->cnt; i++){
        int added = 0;
        for(int j = 0; j < add_file_list->cnt; j++){
            if(!strcmp(add_file_list->lst[j].addres, mem_stage->lst[i].addres)){
                added = 1;
                break;
            }
        }
        if(!added) added = in_stage(mem_stage->lst[i].addres);
        
        if(!added){
            file_list_push_back(add_file_list, &mem_stage->lst[i]);
        }
    }

    add_to_stage(add_file_list);
}

void add_to_stage(FileList *flst){
    FileList *current_stage = get_file_list(get_current_stage_info_addres());
    for(int i = 0; i < flst->cnt; i++){
        if(in_stage(flst->lst[i].addres)){
            int index = find_index_in_file_list_with_addres(
                get_current_stage_info_addres(),
                flst->lst[i].addres
            );
            current_stage->lst[index] = blanck_file();
        }
        char *file_addres_in_stage = get_current_stage_changes_file_addres(get_name_file_in_stage_change(flst->lst[i].addres));
        if(flst->lst[i].state != Delete){
            file_copy(flst->lst[i].addres, file_addres_in_stage);
        } else{
            if(exist_file(file_addres_in_stage)) remove(file_addres_in_stage);
        }
        file_list_push_back(current_stage, &flst->lst[i]);
    }
    set_file_list(get_current_stage_info_addres(), current_stage);

    add_to_mem_stage(flst);
    add_to_undo_file(flst);
}