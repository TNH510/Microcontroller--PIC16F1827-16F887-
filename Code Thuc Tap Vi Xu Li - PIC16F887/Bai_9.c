#include<htc.h>
#include<stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lcd(16).h"

#define _XTAL_FREQ 4000000
__CONFIG(FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);
//FOSC_INTRC_NOCLKOUT
 
void putch(char c);
void tx_init();
void rx_init();
void PWM_init();
void br_init();
void pulse(int data_xung);
void send_char(unsigned char data);
void send_string(const char *s);
char get_char();
char a[1];
int angle_str;
int ecd;

int i = 0;
int dem, pwm_v;
long int xung;
float v;

void main()
{
//Pin setup
	ANSEL = ANSELH = 0; //disable all analog I/O
//Set internal clock to 500kHz. Need to set this first, then setup timer.
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
	char pwm[10];
	char n[5];
	RE1 = 0;
	RE0 = 0;
	TMR2ON = 0;
	//send_string("giao");	//Need to set in order to able to recieve data (tested)
	while(1)
	{
		lcd_gotoxy(0,0);
		printf("TRAN NGOC HIEU");
		for (dem = 0;dem < 1;dem++)
		{
			n[dem] = a[dem];
		}
		send_string(n);
		if(n[0] == 'P')
		{
			RE1 = 0;
			RE0 = 0;
			TMR2ON = 0;
		}
		else if(n[0] == 'S')
		{
			RE0 = 0; RE1 = 1;
			TMR2ON = 1;
			
			pulse(60);
		}
		else if(n[0] == 'R')
		{
			RE0 = 0;
			RE1 = 1;
			pulse(60);
			TMR2ON = 1;
		}
		else if(n[0] == 'L')
		{
			RE0 = 1;
			RE1 = 0;
			pulse(60);
			TMR2ON = 1;
		}
		__delay_ms(300);
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
void pulse(int data_xung)
{
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
		if(i == 1)  i = 0;
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