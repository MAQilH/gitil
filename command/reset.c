#include "../lib/lib.h"
#include "../model/file_list_model.h"

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

void reset(int argc, char* argv[]){
    if(!strcmp(argv[2], "-undo")){
        undo_from_stage();
        return;
    }
}