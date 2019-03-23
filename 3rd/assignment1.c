#include <stdlib.h>
#include <stdio.h>

int countByFor();           //for문으로 계산하는 함수의 선언     
int countByWhile();         //while문으로 계산하는 함수의 선언

int main(){
    int countSpace = 0;     //스페이스의 숫자를 세는 변수
    char str[1001];         //마지막은 nul 들어가야해서 1001칸 배열 필요
    scanf("%[^\n]s",str);   //str에 키보드 입력 받음

    countSpace = countByFor(str);   //for문을 이용해서 공백 세기.
    printf("counted by for      : %d\n", countSpace);   //for문으로 센 스페이스 출력

    countSpace = 0;         //초기화
    countSpace = countByWhile(str); //while문을 이용한 공백세기
    printf("counted by while    : %d\n", countSpace);   //while문으로 센 스페이스 출력

}

int countByFor(char * str){
    int count = 0;          //카운트 변수 선언
    for(int i = 0; str[i] != 0; i++){   //str[i]이 NUL(ascii로 0)이 나올때까지 하나씩 읽으면서 스페이스가 있는지 검사. 
        
        if (str[i]==32){    //ascii코드에서 32(space)를 발견
            count++;        //space발견하면 1 증가
        }

    }
    return count;           //카운트를 반환
}
 
int countByWhile(char * str){
    int count = 0, i = 0;   //카운트 변수와 i변수를 선언하며 초기화
    while(str[i] != 0){
         if (str[i]==32){   //ascii코드에서 32(space)를 발견하면 countSpace변수 숫자 +1
            count++;        //space발견하면 1 증가
        }
        i++;                //i증가
    }
    return count;           //카운트를 반환
    
}
