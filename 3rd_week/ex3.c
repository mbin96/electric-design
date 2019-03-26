#include <stdio.h>

int main(){
    int a = 10;         
    double b = 7.25;    //정수와 부동소수점 변수 선언

    int *pt_a;
    double *pt_b;       //정수와 부동소수점 포인터 선언

    pt_a = &a;
    pt_b = &b;          //포인터에 각 변수의 주소 입력
    printf("sizeof(pt_a) = %d\n",sizeof(pt_a));
    printf("sizeof (pt_b) = %d\n",sizeof(pt_b));    //각 포인터의 사이즈 출력

    printf("int a - %d\n",a);    
    printf("double b = %lf\n\n",b);                 //각 변수의 값 출력
    
    printf("a's address = %p\n", pt_a);
    printf("b's address = %p\n\n",pt_b);            //각 변수의 주소값 출력

    a = 12;  
    *pt_a = 15;         
    b = 8.5;
    *pt_b = 10.75;      //두 변수의 값을 직접 바꾼뒤에 포인터로 역참조하여 바꿔줌

    printf("int a = %d\n",a);                       //각 변수의 값출력
    printf("double b = %lf\n\n",b);                 //이때 변수의 값은 역참조하여 바꾼값으로 출력된

}