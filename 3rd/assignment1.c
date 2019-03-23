#include <stdlib.h>
#include <stdio.h>
//int countByFor();
//int countByWhile();
int main(){
    int countSpace = 0;     //스페이스의 숫자를 세는 변수
    char str[1001];         //마지막은 nul 들어가야해서 1001칸 배열 필요
    scanf("%[^\n]s",str);   //str에 키보드 입력 받음

    for(int i = 0; str[i] != 0; i++){   //하나씩 읽으면서 스페이스가 있는지 검사. 
        
        if (str[i]==32){    //ascii코드에서 32(space)를 발견하면 countSpace변수 숫자 +1
            countSpace++;   
        }

    }
    printf("%d", countSpace);
}