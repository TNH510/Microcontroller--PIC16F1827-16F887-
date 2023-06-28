#include<htc.h>
#include<stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lcd(16).h"

#define _XTAL_FREQ 8000000
__CONFIG(FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);
 
void putch(char c);
void tx_init();
void rx_init();
void PWM_init();
void br_init();
void pulse(int data_xung);
void send_char(unsigned char data);
void send_string(const char *s);
unsigned char min_char[10];
unsigned char sec_char[10];
int count;
unsigned int hour, min,sec;
void main()
{
//Set internal clock to 2MHz. Need to set this first, then setup timer. //Used for setup baud-rate
	IRCF2 = 1; IRCF1 = 1; IRCF0 = 1; //Setup FOSC = 8MHz
	SCS = 1; // Internal oscillator is used for system clock
//Pin setup
	ANSEL = ANSELH = 0; //disable all analog I/O
//Timer0 setup - timer mode
	T0CS =0;
	PSA = 0;
	PS2 = 0; PS1 = 0; PS0 = 1; //Prescale 1:4
	TMR0 = 5;
//Set interrupt for Timer0
	T0IE = 1; T0IF = 0; GIE = 1; PEIE = 1;
//Pin setup
	TRISC6 = 0; //set TX as output
	TRISC7 = 1; //set RX as input
// Setup
	lcd_init();
	tx_init();
	rx_init();
	br_init();
	//send_string("giao");	//Need to set in order to able to recieve data (tested)
	while(1)
	{
		for(hour = 0; hour < 60; hour++)
		{
			for(min = 0; min < 60; min++)
			{
				for (sec = 0; sec < 60; sec++)
				{
					lcd_gotoxy(0,0);
					printf("%02u:%02u:%02u",hour,min,sec);
					sprintf(min_char,"%02u",min);
					sprintf(sec_char,"%02u",sec);
					__delay_ms(1000);

				}
			}
		}	
	}	
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
void interrupt Ngat()
{
	
	if(T0IF)
	{
		count++;
		if(count == 8000){count = 0; send_string(min_char); send_string(":"); send_string(sec_char);send_string("/");}
	}
	TMR0 = 5;
	T0IF = 0;
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
void br_init()
{
	//Baud rate setup
	BRG16 = 1; //Use 16-bit Baud rate 
	BRGH = 1; //High baud rate selected bit
	SPBRGH = 0b00000110; // SPBRG = 1666 (BRG16 = 1; SYNC = 0; BRGH = 1, FOSC = 20MHz)
	SPBRG = 0b10000010; // Baud rate = 1200
}
void putch(char c)
{
	lcd_putc(c);
}