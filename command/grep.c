#include "../lib/lib.h"
#include "../model/string_list_model.h"
#include "commit.h"

char* grep_line(char* line, char* word, int line_num){
    int len = strlen(line);
    char* res = "";
    if(line_num >= 0){
        char tmp[35];
        sprintf(tmp, "\033[1;33m%d:\033[0m ", line_num);
        res = cat_string(res, tmp);
    }
    StringList strl = get_string_list(line, " ");
    int ptr = 0;
    int have = 0;
    for(int i = 0; i < len; i++){
        if(line[i] != ' ' && line[i] != '\t' && (i == 0 || line[i-1] == ' ' || line[i-1] == '\t')){
            if(wildcard_checker(word, strl.lst[ptr])){
                have = 1;
                char tmp[25 + strlen(strl.lst[ptr])];
                sprintf(tmp, "\033[1;31m%s\033[0m", strl.lst[ptr]);
                res = cat_string(res, tmp);
                i += strlen(strl.lst[ptr]) - 1;
            } else{
                char tmp[2];
                sprintf(tmp, "%c", line[i]);
                res = cat_string(res, tmp);
            }
            ptr++;
        } else{
            char tmp[2];
            sprintf(tmp, "%c", line[i]);
            res = cat_string(res, tmp);
        }
    }
    return have? res: NULL;
}

void print_grep_file(char* file_addres, char* word, int line_opt){
    FILE *file = fopen(file_addres, "r");
    char *s = get_string(MAX_STR);
    int line_num = 0;
    while(fgets(s, MAX_STR, file)){
        line_num++;
        char* res = grep_line(s, word, line_opt? line_num: -1);
        if(res != NULL){
            printf("%s", res);
        }
    }
    fclose(file);
}

int grep_file(int argc, char *argv[]){
    char *commit_id = get_current_commit();
    char *word = argv[5];
    char *addres = argv[3];

    addres = cat_string(get_current_addres(), addres);
    if(argc > 6 && !strcmp(argv[6], "-c")) commit_id = argv[7];
    int line_opt = (argc > 8 && !strcmp(argv[8], "-n")) || (argc > 6 && !strcmp(argv[6], "-n"));
    char *file_addres = exist_in_commit(commit_id, addres);
    
    print_grep_file(file_addres, word, line_opt);
    return 1;
}

int grep(int argc, char* argv[]){
    if(!strcmp(argv[2], "-f")){
        return grep_file(argc, argv);
    }
    print_input_invalid();
    return 0;
}