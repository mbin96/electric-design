#include <stdio.h>
#include <stdlib.h>

//구조체를 정의함과 동시에 별칭을 짓는다.
typedef 
struct student{
    char grade;
    int midTerm;
    int finalTerm;
    int attendance;
}STD;


int main(){
    //2-1과 동일하게 STD로 자료형을 선언할수있다.
    STD std1;

    std1.grade = 'a';
    std1.midTerm = 97;
    std1.finalTerm=92;
    std1.attendance = 10;

    printf("std1's grade is %c\n", std1.grade);
    printf("std1's midTerm is %d\n", std1.midTerm);
    printf("std1's final term id %d\n", std1.finalTerm);
    printf("std1's attendance is %d\n", std1.attendance);
    
}