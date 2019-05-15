/*
 * interrupt_example2.c
 *
 * Created: 2016-06-16 
 * Author : webnautes
 */ 
 
#define F_CPU 16000000UL
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
 
 
 
//1. baud rate를 선택
#define USART_BAUDRATE 9600
 
//2.시스템 클록과 원하는 baud rate를 이용하여 UBRR 값을 계산한다.
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
 
 
void usartInit()
{
    //3. UBRR0은 16비트 레지스터이기 때문에 8비트씩 나누어서 넣어야 한다.
    UBRR0H = (uint8_t)(UBRR_VALUE>>8);
    UBRR0L = (uint8_t) UBRR_VALUE;
    
    //4. USART 설정
    UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01); //Charecter size : 8비트
    UCSR0C &= ~(1<<USBS0); //stop  bit : 1비트
    UCSR0C &= ~((1<<UPM01)|(1<<UPM00)); // no parity mode
    
    //5. 송수신을 가능하게 한다.
    UCSR0B=(1<<RXEN0)|(1<<TXEN0);
}
 
void transmitByte(uint8_t data) {
    
    //이전 전송이 끝나기를 기다림
    while(!(UCSR0A&(1<<UDRE0))){};
    
    UDR0 = data;                                            /* send data */
}
 
void printString(const char str[]) {
    uint8_t i = 0;
    while (str[i]) {
        transmitByte(str[i]);
        i++;
    }
}
 
//Interrupt Service Routine for INT0
ISR(INT4_vect)
{
 
 
    PORTF = 0xF0;
    _delay_ms(100);
 
}
 
 
 
int main(void)
{
    usartInit();
    DDRF = 0xF0;         //PORTB3을 출력으로 설정, LED용
 
    
    //falling edge에서 인터러브 발생
    EICRA  &= ~(1<<ISC40);
    EICRA  |= 1<<ISC41;
    
    EIMSK |= 1<<INT4;    //External Interrupt Request 0 Enable
                        //PD0에 스위치 연결, 풀업 저항을 추가한 회로여야 한다.
    
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