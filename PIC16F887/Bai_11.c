#include<htc.h>
#include<stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lcd(16).h"

#define _XTAL_FREQ 4000000
__CONFIG(FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);
 
void putch(char c);
void tx_init();
void rx_init();
void PWM_init();
void br_init();
void pulse(unsigned int duty);
void send_char(unsigned char data);
void send_string(const char *s);
char get_char();
unsigned char a[3];
int n_cvt = 0;
int data_xung;
int i = 0;
int dem;

void main()
{
//Pin setup
	ANSEL = ANSELH = 0; //disable all analog I/O
//Set internal clock to 4MHz. Need to set this first, then setup timer.
	IRCF2 = 1; IRCF1 = 1; IRCF0 = 0; //Setup FOSC = 4MHz
	SCS = 1; // Internal oscillator is used for system clock
//Timer2 setup
	TMR2ON = 1; //Enable Timer2
	T2CKPS1 = 0; T2CKPS0 = 0; // Prescale 1:1 //PWM doesn't involve in postscaler
	TMR2 = 0; PR2 = 66; // using equation in datasheet to find PR2. T = 10^-4s                                                                                                                                                                                                                                                                                                        
// Set global interrupt
	GIE = 1; PEIE = 1;
//Pin setup
	TRISC2 = 0; //set pin C2 as output
	TRISC6 = 0; //set TX as output
	TRISC7 = 1; //set RX as input
	TRISE0 = 0;
	TRISE1 = 0;
// Setup
	lcd_init();
	tx_init();
	rx_init();
	PWM_init();
	br_init();
	unsigned char n[3]; //Need to set at least 1 digit more than the amount of desired array
	//send_string("giao");	//Need to set in order to able to recieve data (tested)
	RE0 = 0; RE1 = 0;
	TMR2ON = 0;
	while(1)
	{
		lcd_gotoxy(0,0);
		printf("TRAN NGOC HIEU");
		for (int dem = 1;dem < 3;dem++)
		{
			n[dem-1] = a[dem];
		}
		n_cvt = atoi(n);
		lcd_gotoxy(0,1);
		printf("PWM = %u Percent",n_cvt);
		send_string(n);
		if(a[0] == 'P')
		{
			RE0 = 0; RE1 = 0;
			TMR2ON = 0;
		}
		else if(a[0] == 'S')
		{
			RE0 = 0; RE1 = 0;
			pulse(n_cvt);
			TMR2ON = 1;
		}
		else if(a[0] == 'R')
		{
			pulse(n_cvt);
			RE0 = 0;
			RE1 = 1;
			TMR2ON = 1;

		}
		else if(a[0] == 'L')
		{
			pulse(n_cvt);
			RE0 = 1;
			RE1 = 0;
			TMR2ON = 1;
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
void pulse(unsigned int duty)
{
	data_xung = duty*(PR2 + 1)/100;
	CCPR1L = data_xung;
//	CCPR1L = data_xung >> 2;
//	DC1B1 = data_xung & 2;
//	DC1B0 = data_xung & 1;
}
void interrupt Ngat()
{
	if(RCIF)
	{
		a[i] = RCREG;
		i++;
		if(i == 3)  i = 0;
		if (RCREG == 'V') i = 0;
	}
	RCIF = 0;
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