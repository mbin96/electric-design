/*
 * 11th-week-timeclock.c
 *
 * Created: 2019-05-15 오전 10:05:34
 * Author : hyeon
 */ 
/////////////////define, include, global variable////////////
//define frequency and chip
#define F_CPU 16000000UL        //atmega128A is 16 MHz
#define __AVR_ATmega128__        //we're using atmega128A

//define STATE number
#define STATE_TIMER_INIT    0    
#define STATE_TIMER_RUNNING    1
#define STATE_TIMER_PAUSE      2

//include
#include <avr/io.h>
#include <avr/interrupt.h>

//global variable declaration
unsigned int state = STATE_TIMER_INIT;                                                      //initialize state
unsigned timeNum = 0, timeIterruptSet = 0;                                                  //variable for time interrupt
unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F}; //7segment decimal LED print with out point
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF}; //7segment decimal LED print with point
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};                                     //7segment's location
unsigned int exp10[4] = {1, 10, 100, 1000};                                                 //print 10^x (for divide timeNum variable)
///////////////////////////////////////////////////////////

///////////////////////////////segment print/////////////////////////////
void printSeg(int num, int segmentLocation){
    //by scemetic, PORTE must being clear before set segment's digit and led.
    //because circuit use same port(A) to set segment's degit and led by latch.
    
    //set segment digit to print
    PORTA = FND_SEGPOS[segmentLocation];
    PORTE = 0x04;                            //set 1 segment digit latch
    PORTE = 0x00;                            //clear port E

    //print num at segment
    PORTA = FND_SEGNP[num];
    PORTE = 0x08;                //set 1 segment led latch
    PORTE = 0x00;                //clear port E
    
}

void initPort(){
    //initialize port A and E
    DDRA = 0xFF;                //set portA(7:0) to output
    DDRE = 0x0C;                //set portE(3:2) to output
    PORTE = 0x04;
}


void initInterrupt(){
    //timer interrupt 
    //normal mode(0), no prescaling
    //when timer being overflow, interrupt exc
    TCCR0 = 0x01;           //normal mode and no prescaling
    TCNT0 = 0x00;
    TIMSK = 0x01;           //overflow 활성화
    TIFR = 0xff;            //초기화 임의 값을 넣으면 하드웨어적으로 초기화
    //external interrupt
    
    //external interrupt int4 enable 
    //int4 버튼 falling edge활성화
    EICRB  &= ~(1<<ISC40);
    EICRB  |= 1<<ISC41;     //same to EICRB = 0d00000011
    //int4 입력 활성화
    EIMSK |= 1<<INT4;

    //external interrupt int5 enable
    //int5 버튼 falling edge활성화
    EICRB  &= ~(1<<ISC50);  //0으로 설정
    EICRB  |= 1<<ISC51;     //1로 설정
    //int5 입력 활성화
    EIMSK |= 1<<INT5;
    
}

////////////////////////interrupt///////////////////
ISR(INT4_vect){
    //external interrupt launch when press int4 BT(falling edge)
    //when state is init or pause, then change state to running
    //when state is runnung, then change state to pause(keep timeNum)
    if(state == STATE_TIMER_INIT || state == STATE_TIMER_PAUSE){
        state = STATE_TIMER_RUNNING;
    }else if(state == STATE_TIMER_RUNNING){
        state = STATE_TIMER_PAUSE;
    }
}

ISR(INT5_vect){
    //external interrupt launch when press int4 BT(falling edge)
    //when state is pause, then reset timeNum(print 0000) and change state to init
    //when state is running, then reset timeNum(print 0000) and keep running
    if(state == STATE_TIMER_PAUSE){
        timeNum = 0;
        state = STATE_TIMER_INIT;
    }else if(state == STATE_TIMER_RUNNING){
        timeNum = 0;
    }
}

ISR(TIMER0_OVF_vect){
    //with out prescaling, time interrupt launch (16 MHz / 256 = 62500) / sec
    //so, 10ms counted when interrupt's 625th launch 
    timeIterruptSet++;
    if((timeIterruptSet > 625) && (state == STATE_TIMER_RUNNING)){
        timeNum++;
        timeIterruptSet = 0;
    }
}
///////////////////inturrupt end////////////////////////

int main(void)
{
    //initialaze port and interrupt
    initPort();
    initInterrupt();

    //Global Interrupt Enable
    sei();      
    
    //main function
    //print 7segment by global variable timeNum
    //timeNum variable is increase by time intrrupt
    while (1){
        //segment print
        for(int i = 0; i < 4; i++){
            printSeg((timeNum/exp10[i])%10,3-i);
        }
        //출력할 수가 9999를 넘으면 초기화
        if(timeNum == 10000) timeNum = 0;
    }
    
}