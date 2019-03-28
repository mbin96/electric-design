#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct student{
    char grade;
    int midTerm;
    int finalTerm;
    int attendance;
}STD;

int main(){
    STD student[10];
    int fi; 
    int sum;

    srand((unsigned)time(NULL));

    for(fi = 0; fi <10; fi++){
        student[fi].midTerm = rand() % 46;
        student[fi].finalTerm = rand() % 46;
        student[fi].attendance = rand() % 11;
        sum = student[fi].midTerm + student[fi].finalTerm + student[fi].attendance;

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

        printf("student %d's grade is %c\n", fi, student[fi].grade);
        printf("student %d's total grade is %c\n", fi, sum);
    }
}