
/////////////////define, include, global variable////////////
//define frequency and chip
#define F_CPU 16000000UL        //atmega128A is 16 MHz
#define __AVR_ATmega128__        //we're using atmega128A

//define STATE number
#define STATE_INIT       0    
#define STATE_RUNNING    1
#define STATE_FORCE_RIGHT 2
#define STATE_FORCE_LEFT 3

//include
#include <avr/io.h>
#include <avr/interrupt.h>

//global variable declaration
unsigned int state = STATE_INIT;                                                            //initialize state
unsigned int timeNum = 0;                                      //variable for time interrupt
unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F}; //7segment decimal LED print with out point
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF}; //7segment decimal LED print with point
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};                                     //7segment's location
unsigned int exp10[4] = {1, 10, 100, 1000}; 
volatile unsigned char stepLeft=0x0c, stepRight=0x0c;
///////////////////////////////////////////////////////////

#define MOTOR_PORT		PORTD
#define MOTOR_PORT_DDR	DDRD
#define SENSOR_PORT		PINC

short motorForceLeftFlag = 0;
short motorForceRightFlag = 0;
unsigned char preSensor = 0;
unsigned char countStopSign = 0;
unsigned char stopSign = 0;

#define STRAIGHT 0
#define RIGHT 1
#define LEFT 2

short forceExcuNum=0;

void initPort(void)
{
	PORTA = 0x00;
	DDRA  = 0x00;
	PORTB = 0x00;
	DDRB  = 0x00;
	PORTC = 0x00;
	DDRC  = 0x00;
	PORTD = 0x00;
	DDRD  = 0xFF;
	PORTE = 0x00;
	DDRE  = 0x00;
	PORTF = 0x00;
	DDRF  = 0x00;
	PORTG = 0x00;
	DDRG  = 0x03;
}

void initDevices(void)
{
	cli(); //disable all interrupts
	XDIV  = 0x00; //xtal divider
	XMCRA = 0x00; //external memory
	initPort();
    

	MCUCR = 0x00;
	EICRA = 0x00; //extended ext ints
	EICRB = 0x00; //extended ext ints
	EIMSK = 0x00;
	TIMSK = 0x00; //timer interrupt sources
	ETIMSK = 0x00; //extended timer interrupt sources
	sei(); //re-enable interrupts
	//all peripherals are now initialized
}

// Stepping Motor derive---------------------------
unsigned char  LEFTmotorOneClock(unsigned char step, char dir)
{	
	step = step & 0x0f;
	if(dir){
		switch(step){
			case 0x09: step=0x01; break;
			case 0x01: step=0x03; break;
			case 0x03: step=0x02; break;
			case 0x02: step=0x06; break;
			case 0x06: step=0x04; break;
			case 0x04: step=0x0c; break;
			case 0x0c: step=0x08; break;
			case 0x08: step=0x09; break;
			default: step=0x0c; break;
		}
	}else{
		switch(step){
			case 0x09: step=0x08; break;
			case 0x01: step=0x09; break;
			case 0x03: step=0x01; break;
			case 0x02: step=0x03; break;
			case 0x06: step=0x02; break;
			case 0x04: step=0x06; break;
			case 0x0c: step=0x04; break;
			case 0x08: step=0x0c; break;
			default: step=0x0c; break;
		}
	}
	return step;

}

unsigned char  RIGHTmotorOneClock(unsigned char step, char dir)
{	
	step = step & 0xf0;
	if(dir){
		switch(step){//후진
			case 0x90: step=0x10; break;
			case 0x10: step=0x30; break;
			case 0x30: step=0x20; break;
			case 0x20: step=0x60; break;
			case 0x60: step=0x40; break;
			case 0x40: step=0xc0; break;
			case 0xc0: step=0x80; break;
			case 0x80: step=0x90; break;
			default: step=0xc0; break;
		}
	}else{
		switch(step){//전진
			case 0x90: step=0x80; break;//1001
			case 0x10: step=0x90; break;//0001
			case 0x30: step=0x10; break;//0011
			case 0x20: step=0x30; break;//0010
			case 0x60: step=0x20; break;//0110
			case 0x40: step=0x60; break;//0100
			case 0xc0: step=0x40; break;//1100
			case 0x80: step=0xc0; break;//1000
			default: step=0xc0; break;
		}
	}
	return step;
}

///////////////////////////////segment print/////////////////////////////
void printSeg(int segNum, int segDigit){
    //by schematic, PORTE must being clear before set segment's digit and led.
    //because circuit use same port(A) to set segment's digit and led by latch.

    //set segment digit to print
    PORTA = FND_SEGPOS[segDigit];
    PORTE |= 0x04; //set 1 segment digit latch
    PORTE &= ~(0x04); //clear LED_CTL(PE2)

    //print num at segment
    PORTA = FND_SEGNP[segNum]; //print with out decimal point
    PORTE |= 0x08; //set 1 segment led latch
    PORTE &= ~(0x08); //clear LED_DATA(PE3)
}

void initSegment(){
	//initialize port A and E
	DDRA = 0xFF;    //set portA(7:0) to output
	DDRE |= 0x0C;   //set portE(3:2) to output
	PORTE |= 0x04;  //init portE(3)
	PORTA = 0x0F;   //init portA(3:0)
}

////////////////////////interrupt///////////////////
void initInterrupt(){
        //external interrupt int4 enable
        //int4 pin falling edge활성화
        EICRB &= ~(1 << ISC40);
        EICRB |= 1 << ISC41; //same to EICRB = 0d00000010
        //enable external interrupt 4
        EIMSK |= 1 << INT4;

        //external interrupt int5 enable
        //int5 pin falling edge활성화
        EICRB &= ~(1 << ISC50); //clear
        EICRB |= 1 << ISC51;    //set
        //enable external interrupt 5
        EIMSK |= 1 << INT5;
}

ISR(INT5_vect){
	
    state = STATE_RUNNING;
	
}
ISR(INT4_vect){
	//when interrupt 4 excu, increase num
	timeNum = OCR0;
	timeNum--;
	if (timeNum<50){
		timeNum = 100;
	}
	OCR0 = timeNum;
}



void initTimerInterrupt()
{
    TCCR0 = 0x0e; //0d00001110//CTC mode and 256 prescaling
    TCNT0 = 0x00; //clear count value register. TCNT0 increase count from 0, clear on 63
    TIMSK = 0x02; //enable Timer/Counter0 compare match interrupt, disable overflow interrupt
    TIFR = 0xff;  //write logic 1 on flag for clear register
    OCR0 = 0x64;  //compare 63
}


void motor(char direction){

	switch(direction){
		case STRAIGHT :stepRight = RIGHTmotorOneClock(stepRight, 1);
		case RIGHT : stepLeft = LEFTmotorOneClock(stepLeft, 0);
			break;
		case LEFT  : stepRight = RIGHTmotorOneClock(stepRight, 1);
			break;
	}
	MOTOR_PORT = stepLeft|stepRight;
}

void sensorScan(int sensor){
    switch(sensor){
        case 0x0f:{  //1111 - 걸리는거없음 직진
			if(stopSign==1){
                stopSign=0;
                countStopSign++;

                if(countStopSign >2){//stopsign 3번 나오면 종료
                    state = STATE_INIT;
                    return;
                }
            }
            motor(STRAIGHT);
			break;
		}
        case 0x0b:{  //1011 --하나걸림 좌회전
            motor(LEFT);
			break;
		}
        case 0x0d:{  //1101 --하나걸림 -우회전
            motor(RIGHT);
            break;
		}
        case 0x07:{  //0111--좌회전 신호
            state=STATE_FORCE_LEFT;
			
			//motor(STRAIGHT);
            break;
		}
        case 0x0e:{  //1110--우회전 신호
            state=STATE_FORCE_RIGHT;
			
			//motor(STRAIGHT);
            break;
		}
		case 0x06:{  //0110--정지 신호
            stopSign=1;
			motor(STRAIGHT);
            break;
		}
		case 0x01:
		case 0x08:
		case 0x00:{  //0000 -- 네개 다 찍힘
            
            motor(STRAIGHT);
            break;
		}
        default: {   
            motor(STRAIGHT);
            break;
		}
    }
}

ISR(TIMER0_COMP_vect)  
{
    if(state == STATE_FORCE_RIGHT){
		if(forceExcuNum<360){
			motor(STRAIGHT);
			forceExcuNum++;
		}else if(forceExcuNum < 630){
			motor(RIGHT);
			forceExcuNum++;
			
		}else{
			forceExcuNum=0;
			state = STATE_RUNNING;
		}
		
		
	}else if(state == STATE_FORCE_LEFT){
		if(forceExcuNum<360){
			motor(STRAIGHT);
			forceExcuNum++;
			}else if(forceExcuNum < 630){
			motor(LEFT);
			forceExcuNum++;
			
			}else{
			forceExcuNum=0;
			state = STATE_RUNNING;
		}
		
	}else if(state==STATE_RUNNING){
		int sensor = SENSOR_PORT & 0x0F;
		sensorScan(sensor);
	}
}






///////////////////interrupt service routine end////////////////////////

int main(void){

    //initialize port and interrupt
    initDevices();
	initSegment();
    initInterrupt();
    initTimerInterrupt();
	
	MOTOR_PORT_DDR = 0xff;
    //Global Interrupt Enable
    sei();
    //main function
    //print 7segment by global variable timeNum
    //timeNum variable is increase by time interrupt
    while (1){
        timeNum = (int)OCR0;
        //segment print
        for(int i = 0; i < 4; i++){
            printSeg((timeNum/exp10[i])%10,3-i);
        }
		
        //출력할 수가 9999를 넘으면 초기화
        
    }
}

////////////코드 최적화//////////////////////
/*
//CTC모드를 사용하고 256프리 스케일링 모드를 사용하여 5번의 인터럽트 실행으로 10ms를 측정하도록 코딩 하였다.
void initTimerInterrupt()
{
    TCCR0 = 0x0e; //0d00001110//CTC mode and 256 prescaling
    TCNT0 = 0x00; //clear count value register. TCNT0 increase count from 0, clear on 124
    TIMSK = 0x02; //enable Timer/Counter0 compare match interrupt, disable overflow interrupt
    TIFR = 0xff;  //write logic 1 on flag for clear register
    OCR0 = 0x7c;  //compare 124
}
ISR(TIMER0_COMP_vect)
{
    timeInterruptExec++;
    if ((timeInterruptExec > 4) && (state == STATE_RUNNING))
    {
        timeNum++;
        timeInterruptExec = 0;
    }
}
*/
////////////////////////////////////////////////