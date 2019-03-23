#include <stdio.h>

void addAndMul(int a, int b, int * aAddB,int * aMulB){
    *aAddB = a + b;
    *aMulB = a * b;

}

void incCBV(int a){
    a++;
}

void incCBR(int *a){
    (*a)++;
}

int main(){
    int a,b;
    int add,mul;

    printf("input a:");
    scanf("%d", &a);

       
    printf("input b:");
    scanf("%d", &b);

    printf("a is %d and b is %d\n\n", a, b);
       
}