#include <stdio.h>
int print_diff_string(char* file_name1, char* file_name2, int num1, int num2, char* s1, char* s2);
int print_diff_file(char* file_addres1, int bg1, int en1, char* file_addres2, int bg2, int en2, FILE* conf_saved_file);
void show_commit_diff(char* commit1, char* commit2);
int diff(int argc, char* argv[]);
int file_have_diff(char* file_addres1, char* file_addres2);