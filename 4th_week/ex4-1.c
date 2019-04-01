#include <stdio.h>
#include <stdlib.h>

int main(){
    //파일포인터 선언
    FILE *fp;
    //스트링 선언
    char *str = "each undeclared identifier is reported only once for each function it appears in The terminal process terminated with exit code: ";
    //텍스트 기반 파일 입출력을 위한 버퍼 선언
    char buf[200];
    int fi;

    //파일포인터에 텍스트 기반 쓰기로 파일을 연다. 실패할경우 NULL이 반환되어 종료된다.
    fp = fopen("test.txt", "wt");
    if(fp == NULL){
        printf("fail to open file\n");
        return -1;
    }

    //파일에 char을 쓴다. 
    fputc('t', fp);
    fputc('e', fp);
    fputc('s', fp);
    fputc('t', fp);
    fputc('\n', fp);
    //파일에 string을 쓴다.
    fputs(str, fp);
    //파일을 닫는다.
    fclose(fp);

    //파일포인터에 파일을 텍스트 기반 읽기로 연다.
    fp = fopen("test.txt","rt");

    for(fi = 0; fi < 5; fi++){
        printf("%c",fgetc(fp));
    }

    fgets(buf,200,fp);
    printf("%s",buf);
    fclose(fp);
    return 0;

}