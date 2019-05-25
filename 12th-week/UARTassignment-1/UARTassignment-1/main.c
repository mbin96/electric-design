/*
 * UARTassignment-1.c
 *
 * Created: 2019-05-23 오후 12:39:30
 * Author : mbin9
 */
#include <avr/io.h>

void initUART(){
    //PE1 - UART output, PE0 - UART input
    DDRE = 0x02;
    
    //Baud rate = 9600
    UBRR0H = 0;
    UBRR0L = 103;
    
    //Receive and transmit enable.
    UCSR0B = 0x18;
}

void uartRx(char * bitRx){
    //wait receive ready
    while((UCSR0A & 0x80) == 0x00);
    *bitRx = UDR0;
}

void uartTx(char * bitTx){
    for(int i = 0; bitTx[i] != '\0'; i++){
        //wait complete of foword transmission 
        while((UCSR0A & 0x20) == 0x00);
        //send bit
        UDR0 = *bitTx;
    }
}

int main(void){
    
    //initialize uart
    initUART();
    char * bitRx, * bitTx;
    
    while (1){
        //receive
        uartRx(bitRx);
        
        //upper case to lower case, lower case to upper case
        *bitTx = *bitRx + 32 - 64*(*bitRx / 97);
        
        //transmission
        uartTx(bitTx);
    }
}

