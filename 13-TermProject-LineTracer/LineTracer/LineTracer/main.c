
/////////////////define, include, global variable////////////
//define frequency and chip
#define F_CPU 16000000UL        //atmega128A is 16 MHz
#define __AVR_ATmega128__        //we're using atmega128A

//define STATE number
#define STATE_INIT       0
#define STATE_RUNNING    1
#define STATE_FORCE_RIGHT 2
#define STATE_FORCE_LEFT 3
#define STATE_CALIB_RIGHT 4
#define STATE_CALIB_LEFT 5

#define MOTOR_PORT        PORTD
#define MOTOR_PORT_DDR    DDRD
#define SENSOR_PORT        PINC

#define STRAIGHT 0
#define RIGHT 1
#define LEFT 2

//include
#include <avr/io.h>
#include <avr/interrupt.h>

//global variable declaration
unsigned int state = STATE_INIT;                                                            //initialize state
unsigned int timeNum = 100;                                      //variable for time interrupt
unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F}; //7segment decimal LED print with out point
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF}; //7segment decimal LED print with point
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};                                     //7segment's location
unsigned int exp10[4] = {1, 10, 100, 1000};
volatile unsigned char stepLeft=0x0c, stepRight=0x0c;
///////////////////////////////////////////////////////////

//flag
unsigned char motorForceLeftFlag = 0;
unsigned char motorForceRightFlag = 0;
unsigned char countStopSign = 0;
unsigned char stopSign = 0;
unsigned char forceRightSign = 0 ;
unsigned char forceLeftSign = 0 ;
unsigned char calibLeft = 0;
unsigned char calibRight = 0;
short countTimeStop = 1000;

void initFlag(){
	motorForceLeftFlag = 0;
	motorForceRightFlag = 0;
	countStopSign = 0;
	stopSign = 0;
	forceRightSign = 0;
	forceLeftSign = 0;
	calibLeft = 0;
	calibRight = 0;
	OCR0 = 100;
}

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

// Stepping Motor drive---------------------------
//1-2상 구동
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

void initTimerInterrupt()
{
	TCCR0 = 0x0d; //0d00001110//CTC mode and 64 prescaling
	TCNT0 = 0x00; //clear count value register. TCNT0 increase count from 0, clear on 63
	TIMSK = 0x02; //enable Timer/Counter0 compare match interrupt, disable overflow interrupt
	TIFR = 0xff;  //write logic 1 on flag for clear register
	OCR0 = 0x6d;  //compare 110
}

//모터 구동
void motor(char direction){

	switch(direction){
		case STRAIGHT :
		stepRight = RIGHTmotorOneClock(stepRight, 1);  //break 없으니까 밑에꺼도 실행됨
		case RIGHT :
		stepLeft = LEFTmotorOneClock(stepLeft, 0);
		break;
		case LEFT  :
		stepRight = RIGHTmotorOneClock(stepRight, 1);
		break;
	}
	
	MOTOR_PORT = stepLeft|stepRight;
}

//출발
ISR(INT5_vect){
	state = STATE_RUNNING;
}

//속도조절

ISR(INT4_vect){
	//when interrupt 4 excu, increase num
	
	if(timeNum > 60){
		timeNum -= 5;
		}else{
		timeNum -= 1;
	}
	if (timeNum < 40){
		OCR0 = timeNum = 110;
	}
	if(timeNum < 100){
		OCR0 = 100;
		}else if(timeNum >=100){
		OCR0 = timeNum;
	}
	
}

char rightSmoth = 0;
char leftSmoth = 0;

void sensorScan(int sensor){
	switch(sensor){
		case 0x0f:{                      //1111 - 걸리는거없음 직진
			//정지신호 있을때
			if(stopSign==1){            // stop sign(0110)이 있은뒤 사라지면 카운트
				forceRightSign = forceLeftSign = 0; //정지신호가 양쪽 조금씩 다르게 들어간경우 예외처리
				stopSign=0;
				countStopSign++;
				countTimeStop = 0;
				if (countTimeStop == 300){
					countStopSign = 0;
				}

				if((countTimeStop < 300)&&(countStopSign > 2)){    //stopsign 3번 나오면 종료
					state = STATE_INIT;
					initFlag();
					return;
				}
				}else if(forceRightSign){
				state=STATE_FORCE_RIGHT;
				}else if(forceLeftSign){
				state=STATE_FORCE_LEFT;
			}
			motor(STRAIGHT);   //직진!
			break;
		}
		case 0x0b:{                      //1011 -- 하나걸림 좌회전
			if(stopSign==1){            // stop sign(0110)이 있은뒤 사라지면 카운트
				forceRightSign = forceLeftSign = 0; //정지신호가 양쪽 조금씩 다르게 들어간경우 예외처리
				stopSign=0;
				countStopSign++;
				countTimeStop = 0;
				if (countTimeStop == 300){
					countStopSign = 0;
				}

				if((countTimeStop < 300)&&(countStopSign > 2)){    //stopsign 3번 나오면 종료
					state = STATE_INIT;
					initFlag();
					return;
				}
			}
			if(leftSmoth == 1){
				motor(STRAIGHT);
				leftSmoth=0;
				}else{
				leftSmoth++;
				motor(LEFT);
			}
			if(forceRightSign){
				state=STATE_FORCE_RIGHT;
				}else if(forceLeftSign){
				state=STATE_FORCE_LEFT;
			}
			break;
		}
		case 0x0d:{                      //1101 -- 하나걸림 -우회전
			if(stopSign==1){            // stop sign(0110)이 있은뒤 사라지면 카운트
				forceRightSign = forceLeftSign = 0; //정지신호가 양쪽 조금씩 다르게 들어간경우 예외처리
				stopSign=0;
				countStopSign++;
				countTimeStop = 0;
				if (countTimeStop == 300){
					countStopSign = 0;
				}

				if((countTimeStop < 300)&&(countStopSign > 2)){    //stopsign 3번 나오면 종료
					state = STATE_INIT;
					initFlag();
					return;
				}
			}
			if(rightSmoth == 1){
				motor(STRAIGHT);
				rightSmoth=0;
				}else{
				rightSmoth++;
				motor(RIGHT);
			}
			if(forceRightSign){
				state=STATE_FORCE_RIGHT;
				}else if(forceLeftSign){
				state=STATE_FORCE_LEFT;
			}
			
			break;
		}
		
		case 0x03:{                     //0011 -- 교차로 좌회전 신호
			forceLeftSign = 1;
			if(leftSmoth == 1){
				motor(STRAIGHT);
				leftSmoth=0;
				}else{
				leftSmoth++;
				motor(LEFT);
			}
			break;
		}        //0011
		case 0x05:{             //0101
			forceLeftSign = 1;
			if(rightSmoth == 1){
				motor(STRAIGHT);
				rightSmoth=0;
				}else{
				rightSmoth++;
				motor(RIGHT);
			}
			break;
		}        //0101
		case 0x07:{                      //0111 -- 교차로 좌회전 신호
			forceLeftSign = 1;
			motor(STRAIGHT);
			break;
		}
		case 0x0c:{
			forceRightSign = 1;
			if(rightSmoth == 1){
				motor(STRAIGHT);
				rightSmoth=0;
				}else{
				rightSmoth++;
				motor(RIGHT);
			}
			break;
		}            //1100
		case 0x0a:{
			forceRightSign = 1;
			if(leftSmoth == 1){
				motor(STRAIGHT);
				leftSmoth=0;
				}else{
				leftSmoth++;
				motor(LEFT);
			}
			break;
		}
		case 0x0e:{                      //1110 -- 교차로 우회전 신호
			
			forceRightSign = 1;
			motor(STRAIGHT);
			break;
		}
		case 0x02://0010
		case 0x04://0100
		case 0x06:{                      //0110 -- 정지 신호
			
			stopSign=1;                    //flag 설정
			motor(STRAIGHT);
			break;
		}
		case 0x01:{                        //0001 - 교차로로 치우쳐 들어감
			if(calibRight == 1){                //교차로 1000으로 들어와서 0001로 나온경우엔 오른쪽으로칼리브레이션을 해줘야 합니다.
				state = STATE_CALIB_RIGHT;
				calibRight = 0;//스테이트 바꿨으면 초기화하기
				return;
				}else{
				calibLeft = 1;
			}
			motor(STRAIGHT);
			break;
		}
		case 0x08:{                        //1000    - 교차로로 치우쳐 들어감
			if(calibLeft == 1){                //교차로 0001으로 들어와서 1000로 나온경우 왼쪽으로칼리브레이션을 해줘야 합니다.
				state = STATE_CALIB_LEFT;
				calibLeft = 0;//스테이트 바꿨으면 초기화하기
				return;
				}else{
				calibRight = 1;
			}
			motor(STRAIGHT);
			break;
		}
		case 0x00:{                      //0000 - 교차로
			forceLeftSign = forceRightSign = stopSign= countStopSign = 0;                //교차로에 많이 치우쳐 들어온경우(0111이 인식된뒤 들어온)교차로 를 나간뒤 STATE_FORCE로 들어가지 않게 초기화 해줘야 합니다.
			motor(STRAIGHT);
			break;
		}
		default: {
			motor(STRAIGHT);
			break;
		}
	}
}
char smothStart=0;
char stateForceSmoth = 0;
short forceStraight = 0;
ISR(TIMER0_COMP_vect)  //OCR0와 카운터 비교해서 실행됨. 즉 모터의 펄스 간격(속도)가 ocr0에 따라 가변
{
	int sensor = SENSOR_PORT & 0x0F; //센서값 읽기
	
	if(countTimeStop < 1000){
		countTimeStop++;
	}
	
	
	if(state == STATE_FORCE_RIGHT){                                 //-강제회전
		if(70 > OCR0&&(smothStart>9)){
			OCR0++;
			smothStart=0;
			}else{
			smothStart++;
		}
		if(!forceRightSign){
			if(stateForceSmoth==3){
				motor(STRAIGHT);
				stateForceSmoth=0;
				}else{
				stateForceSmoth++;
				motor(RIGHT);
			}
			forceStraight++;                                          //교차로에 들어간 뒤엔 회전만 합니다
			if((forceStraight>80)&&(sensor == 0x0d)){
				state = STATE_RUNNING;
				forceStraight=0;                           //강제 회전하다 라인이 잡히면 트랙 타기
			}
			}else{
			if(timeNum<100){
				switch(sensor){
					case 0x0f:
					motor(STRAIGHT);
					break;
					case 0x0b:{                      //1011 -- 하나걸림 좌회전
						if(stateForceSmoth==3){
							motor(STRAIGHT);
							stateForceSmoth=0;
							}else{
							stateForceSmoth++;
							motor(LEFT);
						}
						break;
					}
					case 0x0d:{                      //1101 -- 하나걸림 -우회전
						if(stateForceSmoth==3){
							motor(STRAIGHT);
							stateForceSmoth=0;
							}else{
							stateForceSmoth++;
							motor(RIGHT);
						}
						break;
					}
					default:
					motor(STRAIGHT);
				}
				forceStraight++;
				if(forceStraight==240){
					forceRightSign = 0;
					forceStraight=0;
				}
				}else if(sensor == 0x00 || sensor == 0x01 || sensor == 0x08){
				forceRightSign = 0;
				motor(RIGHT);                               //교차로 나타나면 플래그 초기화 하고 강제로 돌림
				}else{
				switch(sensor){
					case 0x0f:
					motor(STRAIGHT);
					break;
					case 0x0b:{                      //1011 -- 하나걸림 좌회전
						if(stateForceSmoth==3){
							motor(STRAIGHT);
							stateForceSmoth=0;
							}else{
							stateForceSmoth++;
							motor(LEFT);
						}
						break;
					}
					case 0x0d:{                      //1101 -- 하나걸림 -우회전
						if(stateForceSmoth==3){
							motor(STRAIGHT);
							stateForceSmoth=0;
							}else{
							stateForceSmoth++;
							motor(RIGHT);
						}
						break;
					}
					default:
					motor(STRAIGHT);
				}
			}                                      //교차로 들어가기 전까지는 무조껀 직진
		}
		}else if(state == STATE_FORCE_LEFT){                            //-강제회전
		
		if(70 > OCR0&&(smothStart>9)){
			OCR0++;
			smothStart=0;
			}else{
			smothStart++;
		}
		if(!forceLeftSign){
			if(stateForceSmoth==3){
				motor(STRAIGHT);
				stateForceSmoth=0;
				}else{
				stateForceSmoth++;
				motor(LEFT);
			}
			forceStraight++;                                           //교차로에 들어간 뒤엔 회전만 합니다
			if((forceStraight>80)&&(sensor == 0x0b)){                                        //강제 회전하다 라인 잡히면 트랙 타기
				state = STATE_RUNNING;
				forceStraight=0;
			}
			}else{
			if(timeNum<100){
				switch(sensor){
					case 0x0f:
					motor(STRAIGHT);
					break;
					case 0x0b:{                      //1011 -- 하나걸림 좌회전
						if(stateForceSmoth==3){
							motor(STRAIGHT);
							stateForceSmoth=0;
							}else{
							stateForceSmoth++;
							motor(LEFT);
						}
						break;
					}
					case 0x0d:{                      //1101 -- 하나걸림 -우회전
						if(stateForceSmoth==3){
							motor(STRAIGHT);
							stateForceSmoth=0;
							}else{
							stateForceSmoth++;
							motor(RIGHT);
						}
						break;
					}
					default:
					motor(STRAIGHT);
				}
				forceStraight++;
				if(forceStraight==240){
					forceLeftSign = 0;
					forceStraight=0;
					
				}
				}else if(sensor == 0x00 || sensor == 0x01 || sensor == 0x08){
				forceLeftSign = 0;
				motor(LEFT);                               //교차로 나타나면 플래그 초기화 하고 강제로 돌림
				}else{
				switch(sensor){
					case 0x0f:
					motor(STRAIGHT);
					break;
					case 0x0b:{                      //1011 -- 하나걸림 좌회전
						if(stateForceSmoth==3){
							motor(STRAIGHT);
							stateForceSmoth=0;
							}else{
							stateForceSmoth++;
							motor(LEFT);
						}
						break;
					}
					case 0x0d:{                      //1101 -- 하나걸림 -우회전
						if(stateForceSmoth==3){
							motor(STRAIGHT);
							stateForceSmoth=0;
							}else{
							stateForceSmoth++;
							motor(RIGHT);
						}
						break;
					}
					default:
					motor(STRAIGHT);
				}                                        //교차로 들어가기 전까지는 무조껀 직진
			}
		}
		}else if(state == STATE_CALIB_RIGHT){                           //-교차로를 틀어져서 왼쪽으로 들어갈때 칼리브레이션
		if(sensor == 0x0b||sensor == 0x0d){                         //레일 위로 돌아가면 종료
			state = STATE_RUNNING;
			}else if(sensor == 0x0f){                                   //하나도 안걸릴때 일딴 오른쪽으로 돌림
			motor(RIGHT);
			}else{                                                      //교차로를 나오기 전까지는 무조껀 직진
			motor(STRAIGHT);
		}
		}else if(state == STATE_CALIB_LEFT){                            //-교차로를 틀어져서 오른쪽으로 들어갈때 칼리브레이션
		if(sensor == 0x0b||sensor == 0x0d){                         //레일 위로 돌아가면 종료
			state = STATE_RUNNING;
			}else if(sensor == 0x0f){                                   //하나도 안걸릴때 일딴 오른쪽으로 돌림
			motor(LEFT);
			}else{                                                      //교차로를 나오기 전까지는 무조껀 직진
			motor(STRAIGHT);
		}
		}else if(state == STATE_RUNNING){
		if(timeNum < OCR0&&(smothStart > 20)){
			OCR0--;
			smothStart=0;
			}else{
			smothStart++;
		}
		
		sensorScan(sensor);                    //일반적인 라인 팔로잉 상태
	}
}
///////////////////interrupt service routine end////////////////////////

int main(void){

	//initialize port and interrupt
	initDevices();
	initSegment();
	initInterrupt();
	initTimerInterrupt();
	initFlag();
	
	MOTOR_PORT_DDR = 0xff;
	//Global Interrupt Enable
	sei();

	//main function
	//print 7segment by global variable timeNum
	//timeNum variable is state and OCR0
	while (1){
		//segment print
		for(int i = 0; i < 4; i++){
			printSeg(((state*1000 + timeNum)/exp10[i])%10,3-i); //OCR0를 세그먼트에 출력합니다.
		}
		//출력할 수가 9999를 넘으면 초기화
	}
}