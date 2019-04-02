#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//구조체 선언 및 별칭정의
typedef struct student{
    char grade;
    int midTerm;
    int finalTerm;
    int attendance;
}STD;

int main(){

    //구조체 배열의 선언 및 그외 변수 선언
    STD student[10];
    int fi; 
    int sum;

    //랜덤함수 선언
    srand((unsigned)time(NULL));

    //루프문 실행으로 구조체 배열에 값을 넣는다.
    for(fi = 0; fi <10; fi++){

        //배열의 fi번째요소의 중간고사, 기말고사의 값을 랜덤으로 45이하의 값을 넣는다.
        student[fi].midTerm = rand() % 46;
        student[fi].finalTerm = rand() % 46;

        //배열의 fi번째요소의 attendance를 랜덤으로 10 이하로 넣는다
        student[fi].attendance = rand() % 11;
        //출석 점수와 고사점수를 다 합쳐 sum 변수에 임시로 넣는다..
        sum = student[fi].midTerm + student[fi].finalTerm + student[fi].attendance;

        //sum변수에 저장된 점수에 따라 grade값을 넣는다. 
        if(sum>80){
            student[fi].grade = 'A';
        }else if (sum>70){
            student[fi].grade = 'B';
        }else if (sum>60){
            student[fi].grade = 'C';
        }else if (sum>50){
            student[fi].grade = 'D';
        }else{
            student[fi].grade = 'F';
        }

        //학생의 학점과 전체 값을 출력한다.
        printf("student %d's grade is %c\n", fi, student[fi].grade);
        printf("student %d's total grade is %d\n", fi, sum);
    }
}