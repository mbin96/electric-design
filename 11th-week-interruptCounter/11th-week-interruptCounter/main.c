/////////////////define, include, global variable////////////
//define frequency and chip
#define F_CPU 16000000UL  //atmega128A is 16 MHz
#define __AVR_ATmega128__ //we're using atmega128A

//include
#include <avr/io.h>
#include <avr/interrupt.h>

//global variable declaration
unsigned int num = 0;                                                                           //variable for segment print
unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};     //7segment decimal LED print with out point
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF};     //7segment decimal LED print with point
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};                                         //7segment's location
unsigned int exp10[4] = {1, 10, 100, 1000};                                                     //print 10^x (for divide num variable)
///////////////////////////////////////////////////////////

void printSeg(int num, int segDigit){
    //by schematic, PORTE must being clear before set segment's digit and led.
    //because circuit use same port(A) to set segment's digit and led by latch.

    //set segment digit to print
    PORTA = FND_SEGPOS[segDigit];
    PORTE = 0x04; //set 1 segment digit latch
    PORTE = 0x00; //clear port E

    //print num at segment
    if(segDigit == 1) PORTA = FND_SEGWP[num];    //print with decimal point
    else PORTA = FND_SEGNP[num];                 //print with out decimal point
    PORTE = 0x08; //set 1 segment led latch
    PORTE = 0x00; //clear port E
}

void initPort(){
    //initialize port A and E
    DDRA = 0xFF; //set portA(7:0) to output
    DDRE = 0x0C; //set portE(3:2) to output
    PORTE = 0x04; //init port E
    PORTA = 0x0F; //init port A
}

void initInterrupt(){
    //external interrupt

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

//interrupt service routine
ISR(INT4_vect){
    //when interrupt 4 excu, increase num
    //if num == MAX(9999) then clear num
    if(num == 9999){
        num = 0;
    }else{
        num++; 
    }
}

ISR(INT5_vect){
    //when interrupt 4 excu, decrease num
    //if num == MIN(0000) then clear num
    if(num == 0){
        num = 9999;
        }else{
        num--;
    }
}


int main(void){
    //init port and interrupt
    initPort();
    initInterrupt();

    //Global Interrupt Enable
    sei();      
    
    while (1){
        //segment print
        for(int i = 0; i < 4; i++){
            printSeg((num/exp10[i])%10,3-i);
        }
    }
}
