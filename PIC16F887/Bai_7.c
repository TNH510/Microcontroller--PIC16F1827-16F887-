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
void pulse(unsigned int duty);
void send_char(unsigned char data);
void send_string(const char *s);
char a[2];
char n[2];
char xung_char[10];
int width;
int i = 0;
unsigned int data_xung;
void main()
{
//Pin setup
	ANSEL = ANSELH = 0; //disable all analog I/O
//Set internal clock to 2MHz. Need to set this first, then setup timer.
//	IRCF2 = 1; IRCF1 = 0; IRCF0 = 1; //Setup FOSC = 2MHz
//	SCS = 1; // Internal oscillator is used for system clock
//Timer2 setup
	TMR2ON = 1; //Enable Timer2
	T2CKPS1 = 0; T2CKPS0 = 0; // Prescale 1:1 //PWM doesn't involve in postscaler
	TMR2 = 0; PR2 = 249; // using equation in datasheet to find PR2. T = 5*10^-5s
// Set interrupt for Timer2
	TMR2IF = 0; GIE = 1; TMR2IE = 0; PEIE = 1;
//Pin setup
	TRISC2 = 0; //set pin C2 as output
	TRISC6 = 0; //set TX as output
	TRISC7 = 1; //set RX as input
// Setup
	lcd_init();
	//EUSART
	tx_init();
	rx_init();
	br_init();
	//PWM
	PWM_init();
	while(1)
	{
		for(int i = 0; i < 2; i++)
		{
			n[i] = a[i];
		}
		width = atoi(n);
		lcd_gotoxy(0,0);
		printf("%0.3d",data_xung);
		pulse(width);
		sprintf(xung_char,"%d",data_xung);
		send_string(xung_char);
		__delay_ms(500);
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
	data_xung = (duty * (PR2 + 1)) / 100;
	CCPR1L = data_xung;
//	DC1B1 = data_xung & 2;
//	DC1B0 = data_xung & 1;
}
void interrupt Ngat()
{
	if(RCIF)
	{
		a[i] = RCREG;
		i++;
		if(i == 2)  i = 0;
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
	SPBRGH = 0x02; // SPBRG = 520 (BRG16 = 1; SYNC = 0; BRGH = 1, FOSC = 20MHz)
	SPBRG = 0x08; // Baud rate = 9600
}
void putch(char c)
{
	lcd_putc(c);
}