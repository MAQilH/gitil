#include "../lib/lib.h"
#include "../model/file_list_model.h"

char* get_name_file_in_stage_change(File fl){
    return replace_name_with_id(
        fl.addres,
        find_index_in_file_list_with_addres(
            get_current_stage_info_addres(),
            fl.addres
        )
    );
}

int in_stage(char *file_addres){
    return find_in_file_list_with_addres(
        get_current_stage_info_addres(),
        file_addres
    ) != NotFound;
}

void add_to_unstage(FileList flst){
    FILE* unstage_file = fopen(get_current_unstage_info_addres(), "ab");
    fwrite(&flst, sizeof(flst), 1, unstage_file);
    fclose(unstage_file);
}

void add_to_stage(FileList flst){
    FILE* stage_file = fopen(get_current_stage_info_addres(), "rb");
    FileList current_stage;
    fread(&current_stage, sizeof(current_stage), 1, stage_file);
    fclose(stage_file);

    // print_file_list(current_stage);

    FileList flst_added;

    int last_cnt = current_stage.cnt;
    for(int i = 0; i < flst.cnt; i++){
        if(!in_stage(flst.lst[i].addres)){
            flst_added.lst[flst_added.cnt++] = flst.lst[i];
            current_stage.lst[current_stage.cnt] = flst.lst[i];
            if(flst.lst[i].state != Delete){
                file_copy(flst.lst[i].addres, get_current_stage_changes_file_addres(replace_name_with_id(flst.lst[i].addres, current_stage.cnt)));
            }
            current_stage.cnt++;
        }
    }
    print_file_list(flst);

    stage_file = fopen(get_current_stage_info_addres(), "wb");
    fwrite(&current_stage, sizeof(current_stage), 1, stage_file);
    fclose(stage_file);

    add_to_unstage(flst_added);
}

int redo_from_stage(){
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
                remove(
                    get_current_stage_changes_file_addres(
                        get_name_file_in_stage_change(last_added_stage.lst[i])
                    )
                );
            }
        }
        pop_from_file(sizeof(last_added_stage), get_current_unstage_info_addres());

        FILE* stage_file = fopen(get_current_stage_info_addres(), "rb");
        FileList current_stage;
        fread(&current_stage, sizeof(current_stage), 1, stage_file);
        fclose(stage_file);

        current_stage.cnt -= last_added_stage.cnt;

        stage_file = fopen(get_current_stage_info_addres(), "wb");
        fwrite(&current_stage, sizeof(current_stage), 1, stage_file);
        fclose(stage_file);
        return 1;
    }
    fclose(unstage_info_file);
    return 0;
}