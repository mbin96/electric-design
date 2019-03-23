#include <stdio.h>
#include <stdlib.h>
#include <time.h>       //랜덤함수에 시간을 넣어 사용하기위해
#define STDNUM 96       //학생의 숫자 지정

int main(){
    int fi;
    int std[STDNUM]={0,};           //학생 수만큼의 배열을 지정하고 0으로 초기화 한다.
    srand((unsigned)time(NULL));    //랜덤함수 선언

    printf("sizeof(std[0]) = %d\n",sizeof(std[0]));     //배열 요소 한개당의 크기를 출력한다. 여기서는 int
    printf( "sizeof(std) = %d\n", sizeof(std));         //std배열 자체의 크기를 출력한다. 4*96=384byte
    printf("sizeof(std)/sizeof(std[0]) = %d\n",sizeof(std) / sizeof(std[0]));   //배열의 크기에서 int의 크기로 나눈다. 즉 배열요소의 갯수가 된다.

    for(fi=0;fi<STDNUM;fi++){       // 랜덤함수를 이용해 배열에 랜덤한 값을 체운다.
        std[fi] = rand() % 101;     //랜덤함수로 
        printf("student %2d'sgrade is %3d\n", fi, std[fi]);//각각의 배열 요소를 출력
    }
}