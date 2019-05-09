/*
 * GPIOexercise.c
 *
 * Created: 2019-05-09 오후 1:17:54
 * Author : mbin9
 */ 
#define F_CPU 16000000	//프리퀀시
#include <avr/io.h>
#include <util/delay.h>
unsigned char FND_SEGNP[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};//숫자
unsigned char FND_SEGWP[10] = {0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0xA7,0xFF,0xEF};//점있는숫자
unsigned char FND_SEGPOS[4] = {0x01,0x02,0x04,0x08};//세그먼트 자리
unsigned int exp10[4] = {1,10,100,1000};

//포트 초기화

void init_port(){
	DDRF = 0xF0;//led1~4 - 출력
	DDRA = 0xff;//segment - 출력
	DDRE = 0x0c;//segment - 상태
	DDRG = 0x00;//스위치 - 입력
}

//세그먼트 출력하는 함수
//num은 출력할 숫자, whatseg는 4개중 몇번째 세그먼트에 출력할지 선택
void printSeg(int num, int whatSegment){
	//자리수 지정
	PORTA = FND_SEGPOS[whatSegment];
	PORTE = 0x04;
	PORTE = 0x00;
	//세그먼트 숫자출력
	PORTA = FND_SEGNP[num];
	PORTE = 0x08;
	PORTE = 0x00;
	_delay_ms(1);
}

void ex1(){
	_delay_ms(1000);
	//led 4개 켜기
	PORTF = 0xF0;	
	_delay_ms(1000);
	//led 4개 끄기
	PORTF = 0x00;	
}

void ex2(){
	_delay_ms(400);
	PORTF = 0x10;		//led1 출력
	_delay_ms(200);
	PORTF = 0x20;		//led2 출력
	_delay_ms(200);
	PORTF = 0x40;		//led3 출력
	_delay_ms(200);
	PORTF = 0x80;		//led4출력
	_delay_ms(400);
	PORTF = 0x40;		//led3 출력
	_delay_ms(400);
	PORTF = 0x20;		//led2 출력
}

void ex3(){
	int whatSeg = 0, num = 0;
	while(1){
		//seg 자리 오버플로 초기화 하기
		if(whatSeg == 3){
			whatSeg = 0;
		}else{
			whatSeg++;	//오버플로할때 외에는 증가
		}
		//출력 숫자 오버플로 초기화 하기
		if(num == 9){
			num = 0;
		}else{
			num++;		//오버플로 할때 외엔 증가
		}
		//출력하기
		printSeg(num,whatSeg);
		_delay_ms(200);
	}
}

void ex4(){
	int num = 0;
	//falling edge 감지를 위한 이전 스위치 저장 변수
	unsigned char postPING = 0x0f;
	for(;;){
		//범위초과처리
		if (num == 10000){
			num = 0;
		}else if(num == -1){
			num = 9999;
		}
		
		//printSegment
		//(num/exp10[i])%10 << 는 10^i자리수를 출력함
		//1자리수 , 10 자리수, 100자리수, 1000자리수를 순서대로 출력
		for(int i = 0; i < 4; i++){
			printSeg((num/exp10[i])%10,3-i);
		}
		
		//falling edge detect sw input
		//이전 스위치 입력이 없다가 스위치가 입력된경우
		if(PING != 0x0f && postPING == 0x0f){
			if(PING == 0x07) num++;		//when sw8 pressed
			if(PING == 0x0b) num--;		//when sw7 pressed
		}
		postPING = PING;
		
	}
}

int main(void)
{
	//포트 초기화
	init_port();
	int status = 0;
	while (1){
		//ex1();
		//ex2();
		//ex3();
		ex4();
	}
	return 0;
}

