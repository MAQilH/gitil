#include "../lib/lib.h"
#include "../model/file_list_model.h"
#include "stage.h"

void remove_file_in_stage_change(File* fl){
    remove(
        get_current_stage_changes_file_addres(
            get_name_file_in_stage_change(fl->addres)
        )
    );
}

void add_to_undo_file(FileList *flst){
    FILE* unstage_file = fopen(get_current_unstage_info_addres(), "ab");
    fwrite(flst, sizeof(*flst), 1, unstage_file);
    fclose(unstage_file);
}


int undo_from_stage(){
    FILE* unstage_info_file = fopen(get_current_unstage_info_addres(), "rb");
    FileList last_added_stage;
    fseek(unstage_info_file, 0, SEEK_END);
    if(ftell(unstage_info_file) == 0){
        fclose(unstage_info_file);
        return 0;
    }
    int size = sizeof(last_added_stage);
    fseek(unstage_info_file, -size, SEEK_END);
    if(fread(&last_added_stage, sizeof(last_added_stage), 1, unstage_info_file)){
        fclose(unstage_info_file);
        for(int i = 0; i < last_added_stage.cnt; i++){
            if(last_added_stage.lst[i].state != Delete){
                remove_file_in_stage_change(&last_added_stage.lst[i]);
            }
        }
        pop_from_file(sizeof(last_added_stage), get_current_unstage_info_addres());

        FileList current_stage = get_file_list(get_current_stage_info_addres());
        current_stage.cnt -= last_added_stage.cnt;
        set_file_list(get_current_stage_info_addres(), &current_stage);

        return 1;
    }
    fclose(unstage_info_file);
    return 0;
}

void clear_from_stage(FileList *flst){
    FileList stage_file = get_file_list(get_current_stage_info_addres());
    for(int i = 0; i < flst->cnt; i++){
        int index = find_index_in_file_list(&stage_file, flst->lst[i].addres);
        if(index != -1){
            if(flst->lst[i].state != Delete){
                remove_file_in_stage_change(&flst->lst[i]);
            }
            stage_file.lst[index] = blanck_file();
        }
    }
    set_file_list(get_current_stage_info_addres(), &stage_file);
}

void reset(int argc, char* argv[]){
    if(!strcmp(argv[2], "-undo")){
        undo_from_stage();
        return;
    }
    int ptr = 2;
    if(!strcmp(argv[ptr], "-f")) ptr++;
    FileList flst = {.cnt = 0};
    for(; ptr < argc; ptr++){
        char* addres = get_current_addres();
        addres = cat_string(addres, argv[ptr]);
        if(is_directory(addres)){
            get_file_status(&flst, addres, MAX_DEP);
        } else{
            add_file_rel_addres_to_file_list(&flst, addres);
        }
    }
    clear_from_stage(&flst);
}