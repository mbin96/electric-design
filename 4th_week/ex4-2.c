#include <stdio.h>
#include <stdlib.h>

int main(){

    FILE *fp;
    char *str = "each undeclared identifier is reported only once for each function it appears in The terminal process terminated with exit code: ";

    char buf[200];
    int fi;

    fp = fopen("test.txt", "wt");
    if(fp == NULL){
        printf("fail to open file\n");
        return -1;
    }

    fputc('t', fp);
    fputc('e', fp);
    fputc('s', fp);
    fputc('t', fp);
    fputc('\n', fp);

    fputs(str, fp);
    fclose(fp);

    fp = fopen("test.txt","wt");

    
    fclose(fp);
    return 0;

}