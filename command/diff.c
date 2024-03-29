#include "../model/file_list_model.h"
#include "../lib/lib.h"
#include "commit.h"
#include <stdlib.h>
#include <stdio.h>
#define _GNU_SOURCE

State get_changed_state(char* from_addres, char* to_addres){
    if(!exist_file(to_addres)){
        if(!exist_file(from_addres)) return NotFound;
        return Delete;
    }
    if(!exist_file(from_addres)) return Create;
    if(file_cmp(from_addres, to_addres)){
        if(get_premisson_file(from_addres) == get_premisson_file(to_addres))
            return Unchange;
        return Access;
    }
    return Modified;
}

int check_nullspace_string(char* s){
    for(int i = 0; s[i] != '\0'; i++){
        if(s[i] != '\n' && s[i] != ' ' && s[i] != '\t') return 0;
    }
    return 1;
}

void print_line(char* file_name, int line, char* s, void(*print)(char*), int prf, int suf){
    int len = strlen(s);
    if(len && s[len-1] == '\n') s[len-1] = '\0', len--;
    printf("%s-%d\n", file_name, line);
    if(prf == -1 || prf + suf > len){
        print(s);
    } else{
        char *str = get_string(len + 10);
        int ptr = 0;
        for(int i = 0; i < prf; i++) str[ptr++] = s[i];
        if(prf < len - suf) for(int i = 0; i < 4; i++) str[ptr++] = '>';
        for(int i = prf; i < len - suf; i++) str[ptr++] = s[i];
        if(prf < len - suf) for(int i = 0; i < 4; i++) str[ptr++] = '<';
        for(int i = len - suf; s[i] != '\0'; i++) str[ptr++] = s[i];
        print(str);
    }
}

int print_diff_string(char* file_name1, char* file_name2, int num1, int num2, char* s1, char* s2){
    if(s1 != NULL){
        int len1 = strlen(s1);
        if(len1 && s1[len1-1] == '\n') s1[len1-1] = '\0';
    }
    if(s2 != NULL){
        int len2 = strlen(s2);
        if(len2 && s2[len2-1] == '\n') s2[len2-1] = '\0';
    }
    if(s1 != NULL && s2 != NULL){
        if(!strcmp(s1, s2)){
            return 0;
        }
    }
    printf("<<<<<<<<<\n");
    if(s1 == NULL){
        print_line(file_name2, num2, s2, print_fail, -1, -1);
    } else if(s2 == NULL){
        print_line(file_name1, num1, s1, print_success, -1, -1);
    } else{
        int prf = 0;
        while(s1[prf] == s2[prf]) prf++;
        int suf = 0;
        int len1 = strlen(s1), len2 = strlen(s2);
        while(suf < len1 && suf < len2 && s1[len1 - suf - 1] == s2[len2 - suf - 1]) suf++;
        int is_one_word = 1;
        for(int i = prf; i < len1 - suf; i++){
            if(s1[i] == ' ' || s1[i] == '\t'){
                is_one_word = 0;
                break;
            }
        }
        for(int i = prf; i < len2 - suf; i++){
            if(s2[i] == ' ' || s2[i] == '\t'){
                is_one_word = 0;
                break;
            }
        }
        if(is_one_word){
            while(prf && (s1[prf-1] != ' ' && s1[prf-1] != '\t')) prf--;
            while(suf && (s1[len1 - suf] != ' ' && s1[len1 - suf] != '\t')) suf--;
            print_line(file_name1, num1, s1, print_success, prf, suf);
            print_line(file_name2, num2, s2, print_fail, prf, suf);
        } else{
            print_line(file_name1, num1, s1, print_success, -1, -1);
            print_line(file_name2, num2, s2, print_fail, -1, -1);
        }
    }
    printf(">>>>>>>>>\n");
    return 1;
}

int handle_conf_line(char*s1, char*s2, FILE* conf_saved_file, int same){
    char inp = '1';
    if(!same){
        printf("**file 1 -> 1\n**file 2 -> 2\n**quite -> q\n");
        scanf(" %c", &inp);
    }

    int len1 = strlen(s1), len2 = strlen(s2);
    if(len1 && s1[len1-1] != '\n'){
        s1[len1] = '\n';
        s1[len1+1] = '\0';
    }
    if(len2 && s2[len2-1] != '\n'){
        s2[len2] = '\n';
        s2[len2+1] = '\0';
    }

    if(inp == '1'){
        fprintf(conf_saved_file, s1);
    } else if(inp == '2'){
        fprintf(conf_saved_file, s2);
    } else{
        return 0;
    }
    return 1;
}

int print_diff_file(char* file_addres1, int bg1, int en1, char* file_addres2, int bg2, int en2, FILE* conf_saved_file){
    int have_conflict = 0;
    if(en1 < 0) en1 = INF;
    if(en2 < 0) en2 = INF;
    en1++, en2++;
    FILE *file1 = fopen(file_addres1, "r");
    FILE *file2 = fopen(file_addres2, "r");
    char *s1 = get_string(MAX_STR), *s2 = get_string(MAX_STR);
    int lnum1 = bg1, lnum2 = bg2;
    bg1--, bg2--; 
    while(bg1--){
        if(fgets(s1, MAX_STR, file1)) break;
    }
    while(bg2--){
        if(fgets(s2, MAX_STR, file2)) break;
    }
    while(lnum1 <= en1){
        if(fgets(s1, MAX_STR, file1) == NULL){
            lnum1 = INF + 10;
            break;
        }
        if(check_nullspace_string(s1)){
            lnum1++;
            continue;
        }
        while(lnum2 <= en2){
            if(fgets(s2, MAX_STR, file2) == NULL){
                lnum2 = INF + 10;
                break;
            }
            if(check_nullspace_string(s2)){
                lnum2++;
                continue;
            }
            break;
        }
        if(lnum2 == INF + 10) break;

        int same = !print_diff_string(get_file_name(file_addres1), get_file_name(file_addres2), lnum1, lnum2, s1, s2);
        have_conflict |= !same;
        if(conf_saved_file != NULL && !handle_conf_line(s1, s2, conf_saved_file, same)){
            return 0;
        }

        lnum1++, lnum2++;
    }
    int flg = 1;
    while(lnum1 <= en1){
        if(!flg && fgets(s1, MAX_STR, file1) == NULL){
            lnum1 = INF;
            break;
        }
        flg = 0;
        if(check_nullspace_string(s1)){
            lnum1++;
            continue;
        }
        int same = !print_diff_string(get_file_name(file_addres1), NULL, lnum1, 0, s1, NULL);
        have_conflict |= !same;
        if(conf_saved_file != NULL && !handle_conf_line(s1, "\0", conf_saved_file, same)){
            return 0;
        }
        lnum1++;
    }
    while(lnum2 <= en2){
        if(fgets(s2, MAX_STR, file2) == NULL){
            lnum2 = INF;
            break;
        }
        if(check_nullspace_string(s2)){
            lnum2++;
            continue;
        }

        int same = !print_diff_string(NULL, get_file_name(file_addres2), 0, lnum2, NULL, s2);
        have_conflict |= !same;
        if(conf_saved_file != NULL && !handle_conf_line("\0", s2, conf_saved_file, same)){
            return 0;
        }

        lnum2++;
    }
    fclose(file1);
    fclose(file2);
    return have_conflict || conf_saved_file;
}

int diff_string(char* s1, char* s2){
    if(s1 != NULL){
        int len1 = strlen(s1);
        if(len1 && s1[len1-1] == '\n') s1[len1-1] = '\0';
    }
    if(s2 != NULL){
        int len2 = strlen(s2);
        if(len2 && s2[len2-1] == '\n') s2[len2-1] = '\0';
    }
    return strcmp(s1, s2);
}

int file_have_diff(char* file_addres1, char* file_addres2){
    int have_conflict = 0;
    int bg1 = 0, bg2 = 0, en1 = INF, en2 = INF;
    FILE *file1 = fopen(file_addres1, "r");
    FILE *file2 = fopen(file_addres2, "r");
    char *s1 = get_string(MAX_STR), *s2 = get_string(MAX_STR);
    int lnum1 = bg1, lnum2 = bg2;
    while(lnum1 <= en1){
        if(fgets(s1, MAX_STR, file1) == NULL){
            lnum1 = INF + 10;
            break;
        }
        if(check_nullspace_string(s1)){
            lnum1++;
            continue;
        }
        while(lnum2 <= en2){
            if(fgets(s2, MAX_STR, file2) == NULL){
                lnum2 = INF + 10;
                break;
            }
            if(check_nullspace_string(s2)){
                lnum2++;
                continue;
            }
            break;
        }
        if(lnum2 == INF + 10) break;
        have_conflict |= diff_string(s1, s2);
        lnum1++, lnum2++;
    }
    int flg = 1;
    while(lnum1 <= en1){
        if(!flg && fgets(s1, MAX_STR, file1) == NULL){
            lnum1 = INF;
            break;
        }
        flg = 0;
        if(check_nullspace_string(s1)){
            lnum1++;
            continue;
        }
        have_conflict |= 1;
        break;
    }
    while(lnum2 <= en2){
        if(fgets(s2, MAX_STR, file2) == NULL){
            lnum2 = INF;
            break;
        }
        if(check_nullspace_string(s2)){
            lnum2++;
            continue;
        }
        have_conflict |= 1;
        lnum2++;
        break;
    }
    fclose(file1);
    fclose(file2);
    return have_conflict;
}

void show_file_diff(int argc, char* argv[]){
    int bg1 = 1, en1 = INF, bg2 = 1, en2 = INF;
    if(argc > 5 && !strcmp(argv[5], "-line1")){
        sscanf(argv[6], "%d-%d", bg1, en1);
    }
    if(argc > 7 && !strcmp(argv[7], "-line1")){
        sscanf(argv[8], "%d-%d", bg1, en1);
    }
    if(argc > 5 && !strcmp(argv[5], "-line2")){
        sscanf(argv[6], "%d-%d", bg2, en2);
    }
    if(argc > 7 && !strcmp(argv[7], "-line2")){
        sscanf(argv[8], "%d-%d", bg2, en2);
    }
    print_diff_file(argv[3], bg1, en1, argv[4], bg2, en2, NULL);
}


void show_commit_diff(char* commit1, char* commit2){
    FileList *commit_status1 = get_commit_status_file(commit1);
    FileList *commit_status2 = get_commit_status_file(commit2);
    for(int i = 0; i < commit_status1->cnt; i++){
        State state_in_commit2 = find_in_file_list(commit_status2, commit_status1->lst[i].addres);
        int index = find_index_in_file_list(commit_status2, commit_status1->lst[i].addres);
        if(index == -1) state_in_commit2 = Delete;
        if(commit_status1->lst[i].state != Delete && state_in_commit2 != Delete){
            if(file_cmp(exist_in_commit(commit1, commit_status1->lst[i].addres), exist_in_commit(commit2, commit_status1->lst[i].addres)))
                continue;
            print_warn(cat_string("<<<<<<<", cat_string(get_rel_addres(commit_status1->lst[i].addres), ">>>>>>>")));
            print_diff_file(
                exist_in_commit(commit1, commit_status1->lst[i].addres), 1, INF,
                exist_in_commit(commit2, commit_status1->lst[i].addres), 1, INF, NULL
            );
        } else if(commit_status1->lst[i].state != Delete){
            char *s = cat_string(get_rel_addres(commit_status1->lst[i].addres), cat_string(" created in commit ", commit1));
            print_error(s);
        } else if(state_in_commit2 != NotFound){
            char *s = cat_string(get_rel_addres(commit_status2->lst[i].addres), cat_string(" created in commit ", commit2));
            print_error(s);
        }
    }
    for(int i = 0; i < commit_status2->cnt; i++){
        if(find_index_in_file_list(commit_status1, commit_status2->lst[i].addres) == -1 && commit_status2->lst[i].state != Delete){
            char *s = cat_string(get_rel_addres(commit_status2->lst[i].addres), cat_string(" created in commit ", commit2));
            print_error(s);
        }
    }
}

int diff(int argc, char* argv[]){
    if(!strcmp(argv[2], "-f")) show_file_diff(argc, argv);
    else if(!strcmp(argv[2], "-c")) show_commit_diff(argv[3], argv[4]);
    return 1;
}
