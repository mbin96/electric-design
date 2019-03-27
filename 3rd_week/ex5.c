#include <stdio.h>

void addAndMul(int a, int b, int * aAddB,int * aMulB){
    *aAddB = a + b;     //a와 b를 더해서 포인터에 역참조 하여 저장
    *aMulB = a * b;     //a와 b를 곱해서 포인터에 역참조 하여 저장

}

void incCBV(int a){
    a++;                //a=a+1
}

void incCBR(int *a){
    (*a)++;             //a를 역참조하여 연산
}

int main(){
    int a,b;            //a,b 정수 변수 선언
    int add,mul;        //add,mul변수 선언

    printf("input a:"); 
    scanf("%d", &a);    //a변수에 정수 입력받음

       
    printf("input b:"); 
    scanf("%d", &b);    //b변수에 정수 입력받음

    printf("a is %d and b is %d\n\n", a, b);    //각 변수 내의 입력받은 값 출력

    incCBR(&b);         //b의 포인터값을 이용한 b=b+1연산함수 호출
                        //메인 함수의 변수 값이 포인터를 통해 직접 변경됨
    incCBV(a);          //a=a+1연산 함수를 호출
                        //하지만 함수내에서의 로컬변수값 변경은 함수내에서만 유효함
    printf("Now a is %d and b is %d\n\n", a, b);//a와 b값 출력

    addAndMul(a, b, &add, &mul);    //포인터를 이용해 add변수와 mul변수의 값을변경하는 함수 호출

    printf("add : %d\n",add);       //add변수 출력
    printf("mul : %d\n",mul);       //mul뱐수 출력
}