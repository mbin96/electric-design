#include <stdio.h>

int main(){
    int a = 10;
    double b = 7.25;

    int *pt_a;
    double *pt_b;

    pt_a = &a;
    pt_b = &b;
    printf("sizeof(pt_a) = %d\n",sizeof(pt_a));
    printf("sizeof (pt_b) = %d\n",sizeof(pt_b));

    printf("inta - %d\n",a);
    printf("doublr b = %lf\n\n",b);
    printf("a;s address = %p\n", pt_a);
    printf("b's address = %p\n\n",pt_b);

    a= 12;
    *pt_a = 15;

    b = 8.5;
    *pt_b = 10.75;

    printf("int a = %d\n",a);
    printf("double b = %lf\n\n",b);



}