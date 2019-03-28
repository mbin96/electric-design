#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define card_size 100
#define arr_size 100

typedef struct
{
	//////////////////과제시작///////////////////
    char name[arr_size];
    char phone[arr_size];
    char department[arr_size];
	//////////////////과제끝///////////////////
}namecard; // 전화 번호부를위한 구조체

namecard card[card_size]; // 전화 번호부를위한 구조체 배열

int num = 0; // 현재 전화 번호부에 저장된 사람 수.

void clearInputBuffer() // 입력 버퍼 클리어
{
	while(getchar() != '\n');
}

int menu() // 화면에 메뉴를 띄우는 함수
{
	int a;
	printf( "1. add \n");
	printf( "2. search \n");
	printf( "3. delete \n");
	printf( "4. file write \n");
	printf( "5. list view \n");
	printf( "6. quit \n");
	printf( "Select manu :");
	scanf( "%d", & a);
	clearInputBuffer();

	return a;
}

int add() // 전화 번호부에 새로운 사람을 추가하는 함수.
{
	if (num > card_size)
	{
		printf( "ERROR! We can not make more card \n \n");
		return -1;
	}else
	{
		printf( "make new person \n");
		printf( "name");
		scanf( "%[^\n]s", &card[num].name);
        clearInputBuffer();
		printf( "phone :");
		scanf( "%[^\n]s", &card[num].phone);
        clearInputBuffer();
		printf( "department");
		scanf( "%[^\n]s", &card[num].department);
        clearInputBuffer();

		printf( "\n");
		printf( "your input : name : %s \n", card[num].name);
		printf( "your input : phone : %s \n", card[num].phone);
		printf( "your input : department : %s \n", card[num].department);

		num ++;
		printf( "\n");
	}
}

int search()
{
	int a;
	int fi;
	char str[arr_size];
	int find = 0;
	printf( "1. with name \n"); // 이름을 기준으로 전화 번호부를 검색한다.
	printf( "2. with phone number \n"); // 전화 번호를 기준으로 전화 번호부를 검색한다.
	printf( "3. with department \n"); // 부서를 기준으로 전화 번호부를 검색한다.
	printf( "Select menu :");
	scanf( "%d", & a);
	clearInputBuffer();
	switch(a)
	{
	case 1 :
		printf( "\n");
		printf( "what is name?");
		scanf( "%[^\n]s", str);
        clearInputBuffer();
		for (fi = 0; fi <num; fi ++)
		{
			if (!strcmp(str, card[fi].name))
			{
				printf( "\n");
				printf( "----------------------------- \n");
				printf( "card number : %d \n", fi);
				printf( "name : %s \n", card[fi].name);
				printf( "phone : %s \n", card[fi].phone);
				printf( "department : %s \n", card[fi].department);
				find ++;
			}
		}if(find == 0){
			printf( "----------------------------- \n");
			printf( "can not find %s \n", str);
		}else{
			printf( "----------------------------- \n");
			printf( "we find %d person \n", find);
		}
		break;
	    
		///////////////////////과제시작///////////////////////
    case 2 :					//위에 예제 코드에서 strcmp부분만 바꿔주면 된다.
		printf( "\n");
		printf( "what is phoneNumber?");
		scanf( "%[^\n]s", str);			//검색할 폰넘버를 받는부분
        clearInputBuffer();				//%[^\n]s를 이용하면 입력 버퍼에 \n이 남아있으므로 이를 클리어 해준다.

		for (fi = 0; fi < num; fi ++)	//배열의 처음부터 끝까지의 값을 읽어서 일치여부를 확인한다.
		{
			if (!strcmp(str, card[fi].phone))	//strcmp는 값이 일치하면 0이 출력된다. 낫연산자를 이용해 bool연산으로 사용할수있다.
			{									//폰값을 읽어와 입력된값과 비교한다.
				printf( "\n");					
				printf( "----------------------------- \n");
				printf( "card number : %d \n", fi);
				printf( "name : %s \n", card[fi].name);
				printf( "phone : %s \n", card[fi].phone);
				printf( "department : %s \n", card[fi].department);		//일치한경우 찾은 이름 전화번호 부서값을 출력하여준다.
				find ++;						//찾은 결과값을 카운트 하여준다.
			}
		}if (find == 0){	//일치하는 스트링을 찾지 못한경우에 찾지 못했다고 프린트해준다.
			printf( "----------------------------- \n");
			printf( "can not find %s \n", str);
		}else{				//일치하는 스트링을 찾은 경우 일치하는 결과의 갯수를 프린트해준다.
			printf( "----------------------------- \n");
			printf( "we find %d person \n", find);
		}
		break;
	    
    case 3 :					//상동
		printf( "\n");
		printf( "what is department?");
		scanf( "%[^\n]s", str);
        clearInputBuffer();
		for (fi = 0; fi <num; fi ++)
		{
			if (!strcmp(str, card[fi].department)){
				printf( "\n");
				printf( "----------------------------- \n");
				printf( "card number : %d \n", fi);
				printf( "name : %s \n", card[fi].name);
				printf( "phone : %s \n", card[fi].phone);
				printf( "department : %s \n", card[fi].department);
				find ++;
			}
		}if (find == 0){
			printf( "----------------------------- \n");
			printf( "can not find %s \n", str);
		}else{
			printf( "----------------------------- \n");
			printf( "we find %d person \n", find);
		}
		break;
	    //////////////////////////////과제끝//////////////////
    default :
		printf( "check your command \n");
		break;
	}
	printf( "\n");
}

int del()
{
	// 특정리스트 번호의 사람을 찾아서 지운다.
	//(전화 번호를 검색해서 지우는 것이 아니다. namecard card [card_size]; 배열의 인덱스 번호를 기준으로 지운다.)
	// 지우는 동작은 그 번호 뒤에있는 데이터들을 앞으로 덮어 쓰는 것으로 구현한다.

	/////////////////////과제시작//////////////////////
    int delNum;								//scanf를 통해받을 index를 저장할 변수이다.
    printf("please input index nubmer(1~100) to delete >");	//문자열 출력
    scanf("%d",&delNum);					//지울 인덱스의번호를 입력받는다.
    clearInputBuffer();					

    if (delNum >= 1 && delNum < num){		//지우는 인덱스가 1이상이고 num보다 작으면 뒤에있는것을 앞으로 옮긴다.
        for(; delNum < num; delNum++){		//인덱스로부터 끝까지 한칸씩 앞으로 땡김
            card[delNum-1] = card[delNum]; 	//입력받은수 -1을 하는 이유는 받은수의 인덱스는 1부터 시작하기 때문
        }
        num--;			//맨위에 있는 쓰래기 값을 초기화하지는 않고 stack처럼 포인터만 1내린다.
        printf("delete success");			//문자열 출력
    }else if (delNum != 0 && delNum == num){ //지우는 인덱스가 끝 인덱스인 경우에는 파일을 옮기지 않고 넘버만 줄인다.
        printf("delete success");			//0인경우는 아예 자료가 없는경우이므로 포함하지 않아야한다.
        num--;			//위와 동일
    }else{
        printf("index doesn't exist. please check your number\n\n");    
        return -1;		//인덱스가 없으면 에러 출력.
    }
    return 0;
    /////////////////////과제끝///////////////////////
}


int filewrite()
{

    ////////////////////////과제시작//////////////////////////

    FILE *fp;							//입력받은 주소록을 저장하기위한 파일포인터 생성
    fp = fopen("phoneBook.csv","wt");	//주소록을 저장하기위한"phonebook.csv" 파일을 텍스트 기반 쓰기로 파일포인터에 연다.
    if (fp == NULL || num <= 0){			//파일포인터가 파일을 불러오는데 실패하거나 주소록에 입력된게 없을때는 에러반환
        printf("please check your file name and index");
        return -1;						//문자열 출력후 리턴
    }

	fputs("name,phone number,department\n", fp);		//csv파일 맨위에 알아보기 쉽게 값을 넣어줌

    for (int fi = 0; fi < num; fi++){	//루프 카운터를 통해 0부터 num-1까지의 연락처를 파일로 저장함
        fputs(card[fi].name, fp);		//이름을 파일포인터로 넣는다
        fputc(',', fp);					//csv기본 구조는 값을 콤마로 구분
        fputs(card[fi].phone, fp);		//번호를 파일포인터로 넣는다
        fputc(',', fp);					//csv기본 구조는 값을 콤마로 구분
        fputs(card[fi].department, fp);	//부서를 파일포인터로 put한다.
        fputc('\n', fp);				//행을 바꿔준다
	}

    fclose(fp);							//열었던 "phonebook.csv" 파일을 닫는다.
    ///////////////////////과제끝///////////////////////////
	
}

int ls()
{
	// namecard card [card_size];의 모든 값을 화면에 현시한다.
	int fi;
	for(fi = 0; fi <num; fi ++)
	{
		printf( "name : %s \n", card [fi].name);
		printf( "phone : %s \n", card [fi].phone);
		printf( "department : %s \n \n", card [fi].department);
	}
}

int main()
{
	while(1)
	{
		switch(menu())
		{
		case 1 :
			printf( "\n");
			add();
			break;
		case 2 :
			printf( "\n");
			search();
			break;
		case 3 :
			printf( "\n");
			del();
			break;
		case 4 :
			printf( "\n");
			filewrite();
			break;
		case 5 :
			printf( "\n");
			ls();
			break;
		case 6 :
			printf( "Quit! \n");
			return 0;
			break;
		default :
			printf( "check your input \n \n");
			break;
		}
	}
}






