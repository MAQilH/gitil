#include <stdio.h>
#include <time.h>

#define MAX_NAME (260)

typedef struct Alias{
    char from[MAX_NAME];
    char to[MAX_NAME];
} Alias;

int main(){
    FILE* file = fopen("C:\\.gitil\\alias_info.dat", "rb");
    Alias als;
    while(fread(&als, sizeof(als), 1, file)){
        printf("%s %s\n", als.from, als.to);
    }
    fclose(file);
    return 0;
}