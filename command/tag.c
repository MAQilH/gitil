#include "../model/tag_model.h"
#include "../lib/lib.h"

int get_tag(void* tg_void, char* tag_name){
    Tag *tg = tg_void;
    FILE *tag_info = fopen(get_tag_info_addres(), "rb");
    int index = 0;
    while(fread(tg, sizeof(*tg), 1, tag_info)){
        if(!strcmp(tg->tag_name, tag_name)){
            fclose(tag_info);
            return index;
        }
        index++;
    }
    fclose(tag_info);
    return -1;
}

int add_tag(Tag nw, int overwrite){
    Tag cr_tag;
    int index = get_tag(&cr_tag, nw.tag_name);
    if(index == -1){
        FILE *tag_info = fopen(get_tag_info_addres(), "ab");
        fwrite(&nw, sizeof(nw), 1, tag_info);
        fclose(tag_info);
        return 1;
    } else{
        if(overwrite){
            upate_middle_file(&nw, sizeof(nw), index, get_tag_info_addres());
            return 1;
        }
        print_fail("fail: tag is duplicated!");
        return 0;
    }  
}

void show_sorted_tag(){
    int len = get_file_len(get_tag_info_addres(), sizeof(Tag));
    int id[len];
    Tag tg[len];
    FILE *tag_info = fopen(get_tag_info_addres(), "rb");
    for(int i = 0; i < len; i++){
        fread(tg + i, sizeof(Tag), 1, tag_info);
        id[i] = i;
    }
    for(int i = 0; i < len; i++){
        for(int j = 1; j < len; j++){
            if(strcmp(tg[id[j-1]].tag_name, tg[id[j]].tag_name) > -1){
                id[j-1] ^= id[j];
                id[j] ^= id[j-1];
                id[j-1] ^= id[j];
            }
        }
    }
    for(int i = 0; i < len; i++){
        print_tag(tg[id[i]]);
    }
}

int create_tag(int argc, char *argv[]){
    Tag tg;
    strcpy(tg.creator, get_creator());
    strcpy(tg.tag_name, argv[3]);
    tg.date = time(NULL);
    char *msg = get_string(MAX_MESSAGE);
    char *commit_id = get_current_commit();
    int overwrite = 0;
    for(int i = 0; i < 2; i++){
        int ptr = 4 + 2*i;
        if(ptr < argc && !strcmp(argv[ptr], "-m")){
            strcpy(msg, argv[ptr+1]);
        }
        if(ptr < argc && !strcmp(argv[ptr], "-c")){
            strcpy(commit_id, argv[ptr+1]);
        }
        if(ptr < argc && !strcmp(argv[ptr], "-f")){
            overwrite = 1;
        }
    }
    if(argc > 8 && !strcmp(argv[8], "-f")){
        overwrite = 1;
    } 
    strcpy(tg.commit_id, commit_id);
    strcpy(tg.message, msg);
    return add_tag(tg, overwrite);
}

void show_tag(char *tag_name){
    Tag tg;
    get_tag(&tg, tag_name);
    print_tag(tg);
}

int tag(int argc, char *argv[]){
    if(argc == 2){
        show_sorted_tag();
        return 1;
    }
    if(!strcmp(argv[2], "show")){
        show_tag(argv[3]);
        return 1;
    }
    if(!strcmp(argv[2], "-a")){
        return create_tag(argc, argv);
    }
    print_input_invalid();
    return 0;
}