/*
 * 11th-week-timeclock.c
 *
 * Created: 2019-05-15 오전 10:05:34
 * Author : hyeon
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


ISR(INT4_vect)
{
	PORTF = 0xF0;
	_delay_ms(100);
	
}
ISR(INT5_vect)
{
	PORTF = 0x20;
	_delay_ms(200);
	
}


int main(void)
{
	
	DDRF = 0xF0;         //PORTB3을 출력으로 설정, LED용
	
	
	//falling edge에서 인터러브 발생
	EICRB  &= ~(1<<ISC40);
	EICRB  |= 1<<ISC41;
	
	EIMSK |= 1<<INT4;    //External Interrupt Request 0 Enable
	//PD0에 스위치 연결, 풀업 저항을 추가한 회로여야 한다.
	
	EICRB  &= ~(1<<ISC50);
	EICRB  |= 1<<ISC51;
	
	EIMSK |= 1<<INT5;
	
	sei();      //Global Interrupt Enable
	
	while(1)
	{
		_delay_ms(400);
		PORTF = 0x10;        //led1 출력
		_delay_ms(200);
		PORTF = 0x20;        //led2 출력
		_delay_ms(200);
		PORTF = 0x40;        //led3 출력
		_delay_ms(200);
		PORTF = 0x80;        //led4출력
		_delay_ms(400);
		PORTF = 0x40;        //led3 출력
		_delay_ms(400);
		PORTF = 0x20;        //led2 출력
	}
}