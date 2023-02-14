#include<htc.h>
#include<stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lcd(16).h"

#define _XTAL_FREQ 20000000
__CONFIG(FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);
//LCD
void putch(char c);
//EUSART
void tx_init();
void rx_init();
void br_init();
void send_char(unsigned char data);
void send_string(const char *s);
//PWM
void PWM_init(); 
void pulse(int data_xung);
//Global variable setup
int dem;
int count;
unsigned char count_char[10];
unsigned char dem_char[10];
//Main
void main()
{
//Pin setup
	ANSEL = ANSELH = 0; //disable all analog I/O
//Set internal clock to 4MHz. Need to set this first, then setup timer.
	IRCF2 = 1; IRCF1 = 1; IRCF0 = 0; //Setup FOSC = 4MHz
	SCS = 1; // Internal oscillator is used for system clock
//Timer0 setup - counter mode
	T0CS = 1; //Transition on T0CKI pin
	PSA = 1; //Prescaler is assigned to the WDT(watchdog timer)
//	PS2 = 0; PS1 = 1; PS0 = 0; //Prescale 1:8 - PSA = 1 then we dont need to use prescaler
	T0SE = 0; //Increment on high-to-low transition on T0CKI pin
	TMR0 = 0;
//Set interrupt for Timer0
	T0IE = 1; T0IF = 0; GIE = 1; PEIE = 1;
//Pin setup
	TRISA4 = 1;// set A4 as input
// Setup
	//LCD
	lcd_init();
	//EUSART
	tx_init();
	rx_init();
	br_init();
	while(1)
	{
		count = TMR0 + dem*255;
		sprintf(count_char,"%u",count);
		lcd_gotoxy(0,1);
		printf("dem: %04d",count -1);
		if(RA4 == 1){__delay_ms(100);send_string(count_char);}
			
	}	
}
void interrupt Ngat()
{
	
	if(T0IF)
	{
		dem++; 
	}
	T0IF = 0;
}
void send_char(unsigned char data)
{
	while(!TXIF){}
		TXREG = data;

}
void send_string(const char *s)
{
		while(*s)
		{
			send_char(*s++);
		}
}
void tx_init()
{
	SYNC = 0; // Use asynchornous mode // Only in TX setup, RX doesn't have this pin
	TX9 = 0; //8-bit reception
	BRGH = 1; //High speed
	TXEN = 1; //Transmit enabled
}
void rx_init()
{
	RX9 = 0; // 8 bit reception
	SPEN = 1; // TX/RX ports are enabled
	CREN = 1; // Enable receiver (Asynchornous mode)
	//EUSART Receive Interrupt
	RCIE = 1; RCIF = 0;
}
void PWM_init()
{
	CCP1M3 = 1; CCP1M2 = 1; //Enable PWM mode for CCP1
}
void br_init()
{
	//Baud rate setup
	BRG16 = 1; //Use 16-bit Baud rate 
	BRGH = 1; //High baud rate selected bit
	SPBRGH = 0x00; // SPBRG = 103 (BRG16 = 1; SYNC = 0; BRGH = 1, FOSC = 4MHz)
	SPBRG = 0b01100111; // Baud rate = 9600
}
void putch(char c)
{
	lcd_putc(c);
}