#include <stdio.h>
#include <stdlib.h>

typedef struct student
{
    char grade;
    int midTerm;
    int finalTerm;
    int attendance;
}STD;

int main(){
    FILE *fp;
    STD st;

    fp = fopen("student.txt","wt");
    if (fp == NULL){
        printf("fail to open file \n");
    }

    st.grade = 'B';
    st.midTerm = 56;
    st.finalTerm = 72;
    st.attendance = 10;

    fprintf(fp, "grade is %d\n", st.grade);
    fprintf(fp, "midterm is %d\n", st.midTerm);
    fprintf(fp, "finalterm is %d\n", st.finalTerm);
    fprintf(fp, "attendance is %d\n", st.attendance);
    
    fclose(fp);

    return 0;

}