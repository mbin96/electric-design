#include <stdio.h>
#include <stdlib.h>
#include <time.h>           //random 함수를 사용하기위해 include



void statistic(int * arr, int arrSize, float * avr, int * max, int * min){
    float sum = 0;          //총합을 구하는 변수 sum선언과 초기화
    int fi = 0;             //arr를 하나씩 접근하기위한 변수 fi선언과 초기화

 //////////////////////////과제/////////////////////////////
 
    for(; fi < arrSize; fi++){  //avg구하기 시작. arrsize보다 작은동안 실행.
        sum += arr[fi];         //sum변수와 현재 arr값을 더하기
    }
    *avr = sum / arrSize;       //avg구함

     
    //맥스와 민값구하기 시작
    *min = arr[0];              //min값과 max값의 초기값을 arr의 첫번째 값으로 넣어줌.
    *max = arr[0];              //첫값을 int_max나 int_min값으로 지정하고 비교하는것보다 더 효율적이다.
    for(fi = 1; fi < arrSize; fi++){
        if (*max < arr[fi]){    //맥스 값 찾기
            *max = arr[fi];     //현재 저장된값과 비교해서 arr [fi]값이 *max값보다 크면 *max값 갱신
        }
        if (*min > arr [fi]){   //민 값 찾기
            *min = arr [fi];    //비교해서 arr [fi]값이 *min값보다 작으면 *min값 갱신
        }
    }
///////////////////////////과제//////////////////////////

}



int main (int argc, void * argv[]){ //프로그램을 실행할때 argv에 문자열을, argc에 문자열의 개수를 저장 
    int n = 0;                      //실행하면서 받은 문자열의 숫자를 저장할 변수
    int *ptr = NULL;                //포인터 선언과 동시에 포인터의 주소를 NULL(0)으로 초기화 함
    float avr = 0;                  //평균값을 저장할 float변수 선언
    int max = 0, min = 0 , fi;      //맥스값과 최소값을 저장할 변수와 for에서 루프 카운트를 위한 변수 선언

    srand((unsigned)time(NULL));    //랜덤함수 선언

    if(argc < 2){
        printf("CHK cmd Input!!!\n");   //argv가 입력이 되지 않았으면 종료
        return -1;                      //에러 디버깅용 -1 리턴
    }

    n = atoi(argv[1]);                  //argv에 입력받은 char문자열을 int형으로 변환

    if (n<1){
        printf("CHK cmd input!!!!\n");  //argv가 1이하로 입력되었으면 종료
        return -1;                      //에러 디버깅용 -1 리턴
    }

    printf("N = %d\n",n);               //입력받았던 숫자 출력

//////////////////////과제//////////////////////////

    ptr = (int *)calloc( n ,sizeof(int));       //입력받았던 숫자만큼 포인터배열 생성

    for (fi=0; fi<n; fi++){
        ptr[fi] = rand() % 101;                 //랜덤함수를 통해 0부터 100까지의 숫자를 생성하여 포인터에 하나씩넣기
        printf("ptr[%d] = %d\n",fi ,ptr[fi]);   //생성된 랜덤값을 출력
    }
    
    statistic(ptr, n, &avr, &max, &min);        //평균값, 최대 최소값 구하는 함수 호출

//////////////////////과제//////////////////////////

    printf("average : %f\n",avr);       //평균값 출력
    printf("max : %d\n",max);           //맥스값 출력
    printf("min : %d\n",min);           //최소값 출력
    free(ptr);                          //포인터 메모리 반환
    return 0;                           

   

} 