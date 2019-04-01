#include <stdio.h>

//구조체정의
struct student{//예제1과 동일
    char grade;
    int midTerm;
    int finalTerm;
    int attendance;
};

//구조체의 별칭 정의. struct student를 STD로 대신 사용할수있다
typedef struct student STD; 

int main(){
    //구조체의 로컬 변수를 선언할때 자료형을STD라고 하면 선언된다
    STD std1;   
    //구조체 변수 std1의 구성원 선언            
    std1.grade = 'a';
    std1.midTerm = 97;
    std1.finalTerm=92;
    std1.attendance = 10;
    //구조체 변수의 출력
    printf("std1's grade is %c\n", std1.grade);
    printf("std1's midTerm is %d\n", std1.midTerm);
    printf("std1's final term id %d\n", std1.finalTerm);
    printf("std1's attendance is %d\n", std1.attendance);
    
}