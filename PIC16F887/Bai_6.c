#include<htc.h>
#include<stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lcd(16).h"

#define _XTAL_FREQ 20000000
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);
 
void putch(char c);
void tx_init();
void rx_init();
void PWM_init();
void br_init();
void pulse(unsigned int data_xung);
void send_char(unsigned char data);
void send_string(const char *s);
unsigned int count, count_old;
long int xung;
float distance,t,f;
char a[10];

void main()
{
//Pin setup
	ANSEL = ANSELH = 0; //disable all analog I/O
	TRISC1 = 0; //set pin C1 as output //CCP2
	TRISC2 = 1; //set pin C2 as intput //CCP1
	TRISC6 = 0; //set TX as output
	TRISC7 = 1; //set RX as input
//Set internal clock to 2MHz. Need to set this first, then setup timer. //Used for setup baud-rate
//	IRCF2 = 1; IRCF1 = 0; IRCF0 = 1; //Setup FOSC = 2MHz
//	SCS = 1; // Internal oscillator is used for system clock
//Timer1 setup
	T1SYNC = 1;
	TMR1ON = 1; //Enables Timer1
	T1OSCEN = 1; //LP oscillator is enabled for Timer1 clock
	TMR1CS = 0; //Internal clock (FOSC/4)
	T1CKPS1 = 0; T1CKPS0 = 0; //1:1 prescale value
	TMR1GE = 0; //Timer1 is always counting
//Timer2 setup
	TMR2ON = 1; //Enable Timer2
	T2CKPS1 = 0; T2CKPS0 = 0; // Prescale 1:1 //PWM doesn't involve in postscaler
	TMR2 = 0; PR2 = 249; // using equation in datasheet to find PR2. T = 10us
//Set interrupt for CCP1
	CCP1IE = 1; CCP1IF = 0; //When we set this. Do not set TMR1IE and TMR1IF. It will somehow conflict each other
// Set interrupt for Timer2
	TMR2IF = 0; TMR2IE = 0;
// Set global interrupt
	GIE = 1; PEIE = 1;
// Setup
	lcd_init();
	tx_init();
	rx_init();
	PWM_init();
	br_init();
	while(1)
	{
		pulse(20);
		f = 5000000.0/xung;
		sprintf(a,"%f",f);
		send_string(a);
		send_string("/");
		lcd_gotoxy(0,0);
		printf("f: %3.2f",f);
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
void pulse(unsigned int data_xung)
{
	CCPR2L = data_xung;
}
void interrupt Ngat()
{	
	if(CCP1IF)
	{
		xung = abs(CCPR1 - count_old);
		count_old = CCPR1;
	}
	CCP1IF = 0;

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
	CCP2M3 = 1; CCP2M2 = 1; //PWM mode for CCP2
	CCP1M3 = 0; CCP1M2 = 1; CCP1M1 = 0; CCP1M0 = 1; //Capture mode, every rising edge 
	
}
void br_init()
{
	//Baud rate setup
	BRG16 = 1; //Use 16-bit Baud rate 
	BRGH = 1; //High baud rate selected bit
	SPBRGH = 0x02; // SPBRG = 520 (BRG16 = 1; SYNC = 0; BRGH = 1, FOSC = 20MHz)
	SPBRG = 0x08; // Baud rate = 9600
}
void putch(char c)
{
	lcd_putc(c);
}

