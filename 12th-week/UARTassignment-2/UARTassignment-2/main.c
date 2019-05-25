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
///////////////////////////////////////////////////////////

//initialize function
void initSegment(){
    //initialize port A and E
    DDRA = 0xFF;    //set portA(7:0) to output
    DDRE |= 0x0C;   //set portE(3:2) to output
    PORTE |= 0x04;  //init portE(3)
    PORTA = 0x0F;   //init portA(3:0)
}

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

void initExternalInterrupt(){
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

//print 7-segment
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

//uart send ascii, so we must convert intizer to ascii
//intizer num + ascii '0' is ascii num
char * numToChar(int num){
    for(int i = 0; i < 4; i++){
        characterizedNum[3-i] = (char)((num/exp10[i])%10 + '0');
    }
    return characterizedNum;
}

void uartTx(char * bitTx){
    //send string. final bit of string is always NULL
    for(int i = 0; bitTx[i] != '\0'; i++){
        //wait complete of foword transmission 
        while((UCSR0A & 0x20) == 0x00);
        UDR0 = bitTx[i];
    }
    //line break
    while((UCSR0A & 0x20) == 0x00);
    UDR0 = '\r';
    while((UCSR0A & 0x20) == 0x00);
    UDR0 = '\n';
}

void updateCount(int increase){
    //update num
    num += increase;
    //overflow(under flow) prevention
    if(num == 10000)    //when overflow
        num = 0;
    else if(num == -1)  //when underflow
        num = 9999;
    //send count by uart
    uartTx(numToChar(num));
}

//interrupt service routine
ISR(INT4_vect){
    //when interrupt 4 excu, increase num
    updateCount(1);
}

ISR(INT5_vect){
    //when interrupt 5 excu, decrease num
    updateCount(-1);
}

ISR(USART0_RX_vect){
    //update uartRX variable when AVR uart received 
    uartRX = UDR0;
}

int main(void){
    //init port, interrupt and uart
    initUART();
    initSegment();
    initExternalInterrupt();

    //Global Interrupt Enable
    sei();
    
    //send "0000" when start or reset
    uartTx(numToChar(num));
    
    while (1){
        //7-segment print
        for(int i = 0; i < 4; i++)
            printSeg((num/exp10[i])%10, 3-i);
            
        //count by uartrx interrupt routine
        //increase at 'U' or 'u' and decrease at 'D' or 'd'
        if(uartRX == 0x75 || uartRX == 0x55){
            updateCount(1);
            uartRX = '\0';
        }else if(uartRX == 0x64 || uartRX == 0x44){
            updateCount(-1);
            uartRX = '\0';
        }       
    }
}
