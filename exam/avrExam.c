//define, include, global variable
//define frequency and chip
#define F_CPU 16000000UL  //atmega128A is 16 MHz
#define __AVR_ATmega128__ //we're using atmega128A


//include
#include <avr/io.h>
#include <avr/interrupt.h>

//global variable declaration

char characterizedNum[5];
int num = 0;                                                                                    //variable for segment print
unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};     //7segment decimal LED print with out point
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF};     //7segment decimal LED print with point
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};                                         //7segment's location
unsigned int exp10[4] = {1, 10, 100, 1000};                                                     //print 10^x (for divide num variable)
unsigned char uartRX='\0';
unsigned char postPING = 0x0f ;
int timeInterruptExec = 0;
///////////////////////////////////////////////////////////

//--------------------------------initialize function
//아래 초기화함수를 적절하게, 혹은 잘모르겠다 싶으면 전부 메인에 적으세요
//세그먼트 출력 활성화
void initSegment(){
    //initialize port A and E
    DDRA  =  0xFF;    //set portA(7:0) to output
    DDRE  |= 0x0C;   //set portE(3:2) to output
    PORTE |= 0x04;  //init portE(3)
    PORTA =  0x0F;   //init portA(3:0)
}

//uart통신 활성화
void initUART(){
    //PE1 - UART output, PE0 - UART input
    DDRE |= 0x02;       //set PortE(1) in output(1)
    DDRE &= ~(0x01);    //set PortE(0) to input(0)
    
    //set Baud rate = 9600
    UBRR0H = 0;
    UBRR0L = 103;
    
    //Receive and transmit enable.
    UCSR0B = 0x18;
    //enable uart rx interrupt
    UCSR0B |= 0x80;
}

//int5, int4 버튼 활성화
void initExternalInterrupt(){
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

//버튼 입력을 받기위한 초기화
void initButton(){
    DDRG = 0x00;
}

void initTimerInterrupt()
{
    TCCR0 = 0x0e; //0d00001110//CTC mode and 256 prescaling
    TCNT0 = 0x00; //clear count value register. TCNT0 increase count from 0, clear on 124
    TIMSK = 0x02; //enable Timer/Counter0 compare match interrupt, disable overflow interrupt
    TIFR = 0xff;  //write logic 1 on flag for clear register
    OCR0 = 0x7c;  //compare 124
}


//-----------------------------------기능처리 함수

//버튼입력
//아래함수를 실행하면 버튼의 입력에 따라 케이스문에 있는 코드를 실행
void button(){
    //falling edge detect sw input
    //아래에 스위치 한번의 입력당 뭘 할지 쓰세요
    if((PING&0x0f) != 0x0f && postPING == 0x0f){
        switch(PING){
            //sw8
            case(0x07) : //write your code here
                break;
            //sw7
            case(0x0b) : //write your code here
                break;
            //sw6
            case(0x0d) : //write your code here
                break;
            //sw5
            case(0x0e) : //write your code here
                break;
        }
    }
    postPING = PING;
}


//세그먼트 출력. segnum에 들어온 0~9까지의 수를 세그먼트의 segDigit자리에 출력합니다.
//withpoint값이 0인경우 점 없이, withpoint값이 1인경우엔 점 있이 출력합니다.
void printSeg(int segNum, int segDigit, int withPoint){
    //by schematic, PORTE must being clear before set segment's digit and led.
    //because circuit use same port(A) to set segment's digit and led by latch.

    //set segment digit to print
    PORTA = FND_SEGPOS[segDigit];
    PORTE |= 0x04; //set 1 segment digit latch
    PORTE &= ~(0x04); //clear LED_CTL(PE2)

    //print num at segment
    if(withPoint){
        //print with decimal point
        PORTA = FND_SEGWP[segNum]; 
    }else{
        //print with out decimal point
        PORTA = FND_SEGNP[segNum]; 
    }
    PORTE |= 0x08; //set 1 segment led latch
    PORTE &= ~(0x08); //clear LED_DATA(PE3)
}

//uart통신시 4자리의 int형 숫자를 스트링으로 바꿔주는 함수
//리턴값은 3:0 스트링의 포인터 입니다. 
//uart send ascii, so we must convert intizer to ascii
//intizer num + ascii '0' is ascii num
char * numToChar(int num){
    for(int i = 0; i < 4; i++){
        characterizedNum[3-i] = (char)((num/exp10[i])%10 + '0');
    }
    return characterizedNum;
}

//bitTx의 스트링 포인터값을 이용해 스트링을 UART통신으로 컴퓨터에 출력해줍니다.
//스트링의 끝은 NULL('\0')이여야 합니다
void uartTx(char * bitTx){
    //send string. final bit of string is always NULL
    for(int i = 0; bitTx[i] != '\0'; i++){
        //wait complete of foword transmission 
        while((UCSR0A & 0x20) == 0x00);
        UDR0 = bitTx[i];
    }
    //아래부분이 활성화 되면 자동으로 스트링을 보낸뒤 줄넘김 처리를 해줍니다.
    // while((UCSR0A & 0x20) == 0x00);
    // UDR0 = '\r';
    // while((UCSR0A & 0x20) == 0x00);
    // UDR0 = '\n';
}

//signed increase만큼 글로벌 변수 num을 증가 혹은 감소시킵니다. 자동으로 0~9999로 변환시킴
void updateCount(int increase){
    //update num
    num += increase;
    //overflow(under flow) prevention
    if(num > 9999)    //when overflow
        num = num - 10000;
    else if(num < 0)  //when underflow
        num = num + 10000;
    //아래가 활성화 되면 카운트한 넘버를 자동으로 송신 send count by uart
    //uartTx(numToChar(num));
}

//-------------------------------interrupt 처리부
ISR(INT4_vect){
    //when interrupt 4 excu, increase num
    //예시코드
    //아래코드는 int4입력이 들어오면 num값을 1 증가시킵니다.
    updateCount(1);
}

ISR(INT5_vect){
    //when interrupt 5 excu, decrease num
    //예시코드
    //아래코드는 int4입력이 들어오면 num값을 1 감소시킵니다.
    updateCount(-1);
}

//RX인터럽트
//uart수신한 값을 uartRX에 넣습니다.
ISR(USART0_RX_vect){
    //update uartRX variable when AVR uart received 
    uartRX = UDR0;
}

//아래 타이머 인터럽트가 활성화되면 5번 실행될째마다 10ms가 카운트, num값을 1 증가시킴
// ISR(TIMER0_COMP_vect)
// {
//     timeInterruptExec++;
//     if ((timeInterruptExec > 4) && (state == STATE_RUNNING)){
//         //아래의 코드ㅜ는10ms마다 실행됩니다.
//         updateCount(1);
//         timeInterruptExec = 0;
//     }
// }

int main(void){
    //초기화
    //알아서 기능에 맞춰 주석 지우세요
    // initUART();
    // initSegment();
    // initExternalInterrupt();
    // initButton();
    // initTimerInterrupt();

    //Global Interrupt Enable
    //항상 써둘것
    sei();
    
    //uart send "0000" when start or reset
    //uartTx(numToChar(num));
    
    while (1){
        //7-segment 에 num 값을 print
        for(int i = 0; i < 4; i++){
            printSeg((num/exp10[i])%10, 3-i,0);
        }

        //예시코드    
        //count by uartrx interrupt routine
        //increase at 'U' or 'u' and decrease at 'D' or 'd'
        // if(uartRX == 0x75 || uartRX == 0x55){
        //     updateCount(1);
        //     uartRX = '\0';
        // }else if(uartRX == 0x64 || uartRX == 0x44){
        //     updateCount(-1);
        //     uartRX = '\0';
        // }       
    }
}
