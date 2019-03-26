#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int arr[100];           //정수 어레이 선언
    int * ptr_arr;          //포인터 선언
    int fi;                 //루프 카운터 변수 선언

    srand((unsigned)time(NULL));    //랜덤 함수 선언

    ptr_arr = &arr[0];              //포인터에 어레이의 첫번째 요소 주소 입력

    printf("address of arr[0] is %p\n", &arr[0]);       //어레이의 첫요소 주
    printf("arr is pointing %p\n",arr);                 //어레이 자체가 가리키는(포인터) 값의 출력
    printf("ptr_arr is pointing %p\n",ptr_arr);         //포인터의 값을 출력
    printf("\n");
    printf("addressof arr[1] is %p\n", &arr[1]);        //어레이의 두번째 요소 주소
    printf("arr+1 is pointing %p\n", arr+1);            //어레이 자체가 가리키는(포인터) 값 + 1 의 출력
    printf("ptr_aarr+1 is pointing %p\n", ptr_arr+1);   //포인터의 값 + 1 의 출력
    printf("\n");

    for (fi=0; fi < 100; fi++){     //포문을 이용해 랜덤한 0부터 100까지의 정수를 어레이에 넣음
        arr[fi] =rand() % 101;
    }

    printf("arr[0] = %d\n", arr[0]);                    //어레이의 첫요소 값 출력
    printf("*arr = %d\n",*arr);                         //어레이를 역참조하여 값 출력
    printf("*ptr_arr = %d\n",*ptr_arr);                 //포인터를 역참조하여 값출력
    printf("\n");                   
    printf("arr[20] = %d\n", arr[20]);                  //어레이의 21번요소 값 출력
    printf("*(arr+20) = %d\n", *(arr+20));              // 어레이자체의 포인터 + 20 한 값의 역참조값 출력
    printf("*(ptr_arr+20= %d\n",*(ptr_arr+20));         //포인터 + 20 한 값의 역참조값의 출력
    printf("*arr_20 = %d\n",*arr +20);                  //어레의 첫요소 값 + 20 값 출력
    printf("*ptr_arr+20 = %d\n", *ptr_arr + 20);        //포인터의 역참조값 + 20 의 출력

}