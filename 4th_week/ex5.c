#include <stdio.h>
#include <stdlib.h>

//student 구조체 선언 및 별칭 선언
typedef struct student
{
    char grade;
    int midTerm;
    int finalTerm;
    int attendance;
}STD;

int main(){
    FILE *fp;   //파일포인터 선언
    STD st;     //구조체 변수 선언

    //파일포인터에 파일을 연뒤에 열리지 않았을경우 에러메세지 출력
    fp = fopen("student.txt","wt"); //텍스트 쓰기모드로 열기
    if (fp == NULL){
        printf("fail to open file \n");
    }

    //구조체 변수에 값 입력
    st.grade = 'B';
    st.midTerm = 56;
    st.finalTerm = 72;
    st.attendance = 10;

    //구조체변수의 값 파일포인터를 이용해 파일에 출력
    fprintf(fp, "grade is %d\n", st.grade);
    fprintf(fp, "midterm is %d\n", st.midTerm);
    fprintf(fp, "finalterm is %d\n", st.finalTerm);
    fprintf(fp, "attendance is %d\n", st.attendance);
    
    //파일을 닫아줌.
    fclose(fp);

    return 0;

}