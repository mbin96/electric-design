#include <stdio.h>
#include <stdlib.h>

typedef 
struct student{
    char grade;
    int midTerm;
    int finalTerm;
    int attendance;
}STD;


int main(){

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