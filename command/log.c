#include "../model/commit_model.h"
#include "../model/string_list_model.h"
#include "../lib/lib.h"
#include <time.h>

void trav_commit_file_reverse(char* file_addres, int n, void* prm, int(*checker)(Commit*, void*)){
    FILE* file = fopen(file_addres, "rb");
    fseek(file, 0, SEEK_END);
    int ptr = ftell(file);
    Commit cmt;
    while(ptr > 0 && n--){
        ptr -= sizeof(cmt);
        fseek(file, ptr, SEEK_SET);
        fread(&cmt, sizeof(cmt), 1, file);
        if(checker(&cmt, prm)){
            print_commit(cmt);
        }
    }
    fclose(file);
}

int checker1(Commit* cmt, void* prm){
    return 1;
}

int author_checker(Commit* cmt, void* auther){
    return !strcmp(cmt->creator, auther);
}

int since_checker(Commit* cmt, void* date){
    int *d = date;
    return cmt->date >= *d;
}

int before_checker(Commit* cmt, void* date){
    int *d = date;
    return cmt->date < *d;
}

int search_checker(Commit* cmt, void* str){
    StringList cmt_lst_ = get_string_list(cmt->message, " ");
    StringList *cmt_lst = &cmt_lst_;
    StringList *str_lst = str;
    for(int i = 0; i < cmt_lst->cnt; i++){
        for(int j = 0; j < str_lst->cnt; j++){
            if(wildcard_checker(str_lst->lst[j], cmt_lst->lst[i])){
                return 1;
            }
        }
    }
    return 0;
}

int get_epoch(char* date){
    struct tm d;
    sscanf(date, "%d-%d-%d/%d:%d:%d", &d.tm_year, &d.tm_mon, &d.tm_mday, &d.tm_hour, &d.tm_min, &d.tm_sec);
    
    d.tm_isdst = -1;
    d.tm_year -= 1900;
    d.tm_mon--;
    return mktime(&d);
}

int logg(int argc, char* argv[]){
    if(argc == 2){
        trav_commit_file_reverse(get_commits_info_addres(), INF, NULL, checker1);
        return 1;
    } 
    char *act = argv[2];
    if(!strcmp(act, "-n")){
        trav_commit_file_reverse(get_commits_info_addres(), stoi(argv[3]), NULL, checker1);
        return 1;
    } else if(!strcmp(act, "-author")){
        trav_commit_file_reverse(get_commits_info_addres(), INF, argv[3], author_checker);
        return 1;
    } else if(!strcmp(act, "-branch")){
        trav_commit_file_reverse(get_commit_info_addres(argv[3]), INF, NULL, checker1);
        return 1;
    } else if(!strcmp(act, "-before")){
        int date = get_epoch(argv[3]);
        trav_commit_file_reverse(get_commits_info_addres(), INF, &date, before_checker);
        return 1;
    } else if(!strcmp(act, "-since")){
        int date = get_epoch(argv[3]);
        trav_commit_file_reverse(get_commits_info_addres(), INF, &date, since_checker);
        return 1;
    } else if(!strcmp(act, "-search")){
        StringList strl = {.cnt = 0};
        strl.lst = (char**)calloc(argc, sizeof(char*));
        for(int i = 3; i < argc; i++){
            strl.lst[strl.cnt++] = argv[i];
        }
        trav_commit_file_reverse(get_commits_info_addres(), INF, &strl, search_checker);
        return 1;
    }
    return 0;
}