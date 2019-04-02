#include <stdio.h>

//student구조체의 정의와 선언
struct student{         
    char grade;
    int midTerm;
    int finalTerm;
    int attendance;
};
//메인함수 시작
int main(){
    struct student std1;    //함수내에서 구조체 변수 정의

    std1.grade = 'a';
    std1.midTerm = 97;
    std1.finalTerm=92;
    std1.attendance = 10;   //구조체 변수의 선언

    //구조체 변수의 구성원 출력
    printf("std1's grade is %c\n", std1.grade);
    printf("std1's midTerm is %d\n", std1.midTerm);
    printf("std1's final term id %d\n", std1.finalTerm);
    printf("std1's attendance is %d\n", std1.attendance);
    
}
