#include <stdio.h>

int main(){
    char str[100];  //스트링 받을 배열 생성. 99자까지 받을수 있다.
    printf("please input your text : ");    //문자열 출력
    scanf("%[^\n]s", str);    //[^\n]string입ㅇ력을 받을때, 엔터가 나올때까지 한꺼번에 받는의미
    printf("your text is : %s", str);   //배열에 입력받은 스트링을 출력

}