#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int fi;                     //루프 카운터 변수 선언
    int * ptr = NULL;           //포인터를 선언하고 초기화
    int size = 0;               //size변수 선언

    srand((unsigned)time(NULL));//랜덤함수 선언
    
    while (size <= 0){          //size가 0보다 작은동안 실행
        printf("size for memory : ");
        scanf("%d", &size);     //사이즈 값을 입력 받음
        
        if (size <=0 ){         //사이즈값이 0보다 작으면 경고 출력
            printf("\n size must be bigger then 0\n");
        }
    }

    ptr = (int *)malloc(size*sizeof(int));      //포인터 공간 할당

    if(ptr == NULL){
        printf("FAIL : CANNOT ASSIGN!!\n");     //포인터 배열이 잘못되었을때의 예외처리
        return -1;              //디버깅용 리턴값
    }

    for (fi = 0; fi < size; fi++ ){
        ptr[fi] = rand() % 101;                 //포인터의 배열에 0~100의 값 랜덤으로 입력
        printf("ptr[%3d] = %3d\n", fi, ptr[fi]);//입력된값 출력

    }
    free(ptr);                  //포인터에 할당된 공간을 지워줌
    return 0;                   //0리턴

}