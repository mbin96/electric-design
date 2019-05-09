/*
 * GPIOexercise.c
 *
 * Created: 2019-05-09 오후 1:17:54
 * Author : mbin9
 */ 
#define F_CPU 16000000
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

//j값을 각 자리수로 나눠 세그먼트당 출력하는 함수.
void printSeg(int num, int whatSegment){
	PORTA = FND_SEGPOS[whatSegment];
	PORTE = 0x04;
	PORTE = 0x00;
	PORTA = FND_SEGNP[num];
	PORTE = 0x08;
	PORTE = 0x00;
	_delay_ms(1);
}

void ex1(){
	_delay_ms(1000);
	PORTF = 0xF0;
	_delay_ms(1000);
	PORTF = 0x00;
}

void ex2(){
	_delay_ms(400);
	PORTF = 0x10;
	_delay_ms(200);
	PORTF = 0x20;
	_delay_ms(200);
	PORTF = 0x40;
	_delay_ms(200);
	PORTF = 0x80;
	_delay_ms(400);
	PORTF = 0x40;
	_delay_ms(400);
	PORTF = 0x20;
}

void ex3(){
	int i = 0, j = 0;
	for(;;){
		if(i == 3) i = 0;
		else i++;
		if(j == 9) j = 0;
		else j++;
		printSeg(j,i);
		_delay_ms(200);
	}
}

void ex4(){
	int num = 0;
	unsigned char postPING = 0x0f;
	for(;;){
		//범위초과처리
		if (num == 10000){
			num = 0;
		}else if(num == -1){
			num = 9999;
		}
		//printSegment
		for(int i = 0; i < 4; i++){
			printSeg((num/exp10[i])%10,3-i);
		}
		//detect sw input
		
		
		
		if(PING != 0x0f && postPING == 0x0f){
			if(PING == 0x07) num++;		//when sw8 pressed
			if(PING == 0x0b) num--;		//when sw7 pressed
			if(PING == 0x0e) return 0;	//when sw5 pressed
		}
		postPING = PING;
		
	}
	
}

void helloClock(){
	int mini = 0;
	unsigned char postPING = 0x0f;
	for(;;){
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		//범위초과처리
		if (num == 10000){
			num = 0;
			}else if(num == -1){
			num = 9999;
		}
		//printSegment
		for(int i = 0; i < 4; i++){
			printSeg((num/exp10[i])%10,3-i);
		}
		//detect sw input
		
		
		
		if(PING != 0x0f && postPING == 0x0f){
			if(PING == 0x07) num++;		//when sw8 pressed
			if(PING == 0x0b) num--;		//when sw7 pressed
			if(PING == 0x0e) return 0;	//when sw5 pressed - reset
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

