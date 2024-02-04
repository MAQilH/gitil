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
    File balnck = blanck_file();
    fwrite(&balnck, sizeof(File), 1, unstage_file);
    fwrite(flst->lst, sizeof(File), flst->cnt, unstage_file);
    fclose(unstage_file);
}

int undo_from_stage(){
    int len = get_file_len(get_current_unstage_info_addres(), sizeof(File));
    FILE* unstage_info_file = fopen(get_current_unstage_info_addres(), "rb");
    int ptr = len*sizeof(File);
    if(len == 0){
        fclose(unstage_info_file);
        return 0;
    }
    int cnt = 0;
    File added_file;
    while(ptr){
        cnt++;
        ptr -= sizeof(added_file);
        fseek(unstage_info_file, ptr, SEEK_SET);
        fread(&added_file, sizeof(File), 1, unstage_info_file);
        if(is_blanck(&added_file)) break;
        if(added_file.state != Delete){
            remove_file_in_stage_change(&added_file);
        }
    }
    fclose(unstage_info_file);
    
    FileList *unstage_file_ist = get_file_list(get_current_unstage_info_addres());
    unstage_file_ist->cnt -= cnt;
    set_file_list(get_current_unstage_info_addres(), unstage_file_ist);

    FileList *current_stage = get_file_list(get_current_stage_info_addres());
    current_stage->cnt -= cnt-1;
    set_file_list(get_current_stage_info_addres(), current_stage);
    return 0;
}

void clear_from_stage(FileList *flst){
    FileList *stage_file = get_file_list(get_current_stage_info_addres());
    for(int i = 0; i < flst->cnt; i++){
        int index = find_index_in_file_list(stage_file, flst->lst[i].addres);
        if(index != -1){
            if(flst->lst[i].state != Delete){
                remove_file_in_stage_change(&flst->lst[i]);
            }
            stage_file->lst[index] = blanck_file();
        }
    }
    set_file_list(get_current_stage_info_addres(), stage_file);
}

void reset(int argc, char* argv[]){
    if(argc == 2){
        
        return;
    }
    if(!strcmp(argv[2], "-undo")){
        undo_from_stage();
        return;
    }
    int ptr = 2;
    if(!strcmp(argv[ptr], "-f")) ptr++;
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
        } else{
            add_file_rel_addres_to_file_list(flst, addres);
        }
    }
    printf("%d files reset!\n", flst->cnt);
    clear_from_stage(flst);
}