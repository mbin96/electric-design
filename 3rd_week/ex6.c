#include <stdio.h>

int main(int argc,char*argv[]){//프로그램을 실행할때 argv에 문자열을, argc에 문자열의 개수를 저장
    int fi;                     //루프 카운트 변수 설정

    printf("main function get %d argv\n",argc);     //문자열의 갯수 출력
    for (fi = 0; fi < argc; fi++){  
        printf("argv[%d] : %s\n", fi, argv[fi]);    //argv출력
    }
}