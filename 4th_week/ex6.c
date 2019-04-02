#include <stdio.h>
#include <stdlib.h>
//버퍼사이즈의 정의
#define BUFSIZE 100

int main(){
    //오리지널과 카피할 파일 포인터 두개 선언
    FILE *ori;
    FILE *cp;

    //카피하는데 사용할 버퍼 생성
    long buffer[BUFSIZE];
    int count;

    //바이너리 파일 읽기와 쓰기로 각각 포인터에 파일을 연다.
    ori = fopen("ori.png","rb");
    cp = fopen("copy.png","wb");

    //포인터에 파일이 열리지 않은경우 에러메세지와 함께 종료
    if (ori == NULL || cp == NULL){
        printf("fail to open file\n");
        return -1;
    }

    //파일이 EOF일때까지 계속 버퍼만큼 읽고 쓰는것을 반복한다.
    while (1){
        //버퍼 만큼 읽는다
        count = fread(buffer, sizeof(long), BUFSIZE, ori);
        //파일이 버퍼만큼 읽히지 않은경우 즉 파일이 끝났거나 읽기 작업에 에러가 생긴경우이다
        if(count < BUFSIZE){
            //EOF 인 경우 
            if(feof(ori) != 0){
                fwrite(buffer, sizeof(long),count, cp);
                printf("copy success!!\n");
                break;
            }else{
                printf("copt fail\n");
                break;
            }
        }else{
            //잘 읽는중인경우 계속해서 쓰기 작업을 계속한다.
            fwrite(buffer, sizeof(long), BUFSIZE, cp);
        }
    }

    //파일포인터를 반환하고 프로그램을 종료한다.
    fclose(ori);
    fclose(cp);
    return 0;

}