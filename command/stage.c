#include "../lib/lib.h"
#include "../model/file_list_model.h"

char* get_name_file_in_stage_change(File* fl){
    return replace_name_with_hash(fl->addres);
}

void remove_file_in_stage_change(File* fl){
    remove(
        get_current_stage_changes_file_addres(
            get_name_file_in_stage_change(fl)
        )
    );
}

int in_stage(char *file_addres){
    return find_in_file_list_with_addres(
        get_current_stage_info_addres(),
        file_addres
    ) != NotFound;
}

void add_to_mem_stage(FileList* flst){
    FILE* mem_stage_file = fopen(get_current_mem_stage_info_addres(), "rb");
    FileList mem_stage;
    fread(&mem_stage, sizeof(mem_stage), 1, mem_stage_file);
    fclose(mem_stage_file);

    for(int i = 0; i < flst->cnt; i++){
        mem_stage.lst[mem_stage.cnt++] = flst->lst[i];
    }

    mem_stage_file - fopen(get_current_mem_stage_info_addres(), "wb");
    fwrite(&mem_stage, sizeof(mem_stage), 1, mem_stage_file);
    fclose(mem_stage_file);
}

void redo(){
    FILE* mem_stage_file = fopen(get_current_mem_stage_info_addres(), "rb");
    FileList mem_stage;
    fread(&mem_stage, sizeof(mem_stage), 1, mem_stage_file);
    fclose(mem_stage_file);

    FileList add_file_list = {.cnt = 0};
    for(int i = 0; i < mem_stage.cnt; i++){
        int added = 0;
        for(int j = 0; j < add_file_list.cnt; j++){
            if(!strcmp(add_file_list.lst[j].addres, mem_stage.lst[i].addres)){
                added = 1;
                break;
            }
        }
        if(!added) added = in_stage(mem_stage.lst[i].addres);
        
        if(!added){
            add_file_list.lst[add_file_list.cnt++] = mem_stage.lst[i];
        }
    }

    add_to_stage(&add_file_list);
}

void add_to_stage(FileList *flst){
    FILE* stage_file = fopen(get_current_stage_info_addres(), "rb");
    FileList current_stage;
    fread(&current_stage, sizeof(current_stage), 1, stage_file);
    fclose(stage_file);

    for(int i = 0; i < flst->cnt; i++){
        current_stage.lst[current_stage.cnt] = flst->lst[i];
        if(in_stage(flst->lst[i].addres)){
            int index = find_index_in_file_list_with_addres(
                get_current_stage_info_addres(),
                flst->lst[i].addres
            );
            current_stage.lst[index] = blanck_file();
        }
        if(flst->lst[i].state != Delete){
            file_copy(flst->lst[i].addres, get_current_stage_changes_file_addres(replace_name_with_hash(flst->lst[i].addres)));
        }
        current_stage.cnt++;
    }

    // print_file_list(current_stage);
    // current_stage = get_clean_file_list(&current_stage);
    // print_file_list(current_stage);
    
    stage_file = fopen(get_current_stage_info_addres(), "wb");
    fwrite(&current_stage, sizeof(current_stage), 1, stage_file);
    fclose(stage_file);

    add_to_mem_stage(flst);
    add_to_undo_file(flst);
}