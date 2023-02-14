#include<htc.h>
#include<stdio.h>
#include <math.h>
#include <stdlib.h>

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
float distance,t;
char a[10];
int i;

void main()
{
//Pin setup
	ANSEL = ANSELH = 0; //disable all analog I/O
	TRISC2 = 0; //set pin C2 as output
	TRISD5 = 0; //set D5 as output
	TRISD6 = 0; //set D7 as output
	TRISD7 = 0; //set D7 as output
	RC2 = RD5 = RD6 = RD7 = 0; 
//Set internal clock to 2MHz. Need to set this first, then setup timer. //Used for setup baud-rate
//	IRCF2 = 1; IRCF1 = 0; IRCF0 = 0; //Setup FOSC = 1MHz
//	SCS = 1; // Internal oscillator is used for system clock
//Timer2 setup
	T2CKPS1 = 0; T2CKPS0 = 1; // Prescale 1:4 //PWM doesn't involve in postscaler
	TMR2 = 0; PR2 = 124; // using equation in datasheet to find PR2. T = 10^-4s
	TMR2ON = 1; //Enable Timer2
// Set global interrupt
	GIE = 1; PEIE = 1;
// Setup
	tx_init();
	PWM_init();
	rx_init();
	br_init();
	char a[10];
	while(1)
	{}	
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
	if(RCIF)
	{
		a[i] = RCREG;
		i++;
		if(i == 1)  i = 0;
		if(a[0] == 'S')
		{
			i = 0;
			PWM_init();
			P1M1 = 0; P1M0 = 1;		
			pulse(124);
		}
		if(a[0] == 'P')
		{
			i = 0;
			CCP1CON = 0b00000000;		
			pulse(0);
		}
		if(a[0] == 'L')
		{
			P1M1 = 0; P1M0 = 1;	
			i = 0;
			pulse(124);
		}
		if(a[0] == 'R')
		{
			P1M1 = 1; P1M0 = 1;	
			i = 0;
			pulse(124);
		}
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
void pulse(unsigned int data_xung)
{
	CCPR1L = data_xung;
//	DC1B1 = data_xung & 2;
//	DC1B0 = data_xung & 1;
}
void PWM_init()
{
	CCP1M3 = 1; CCP1M2 = 1; //PWM mode for CCP2	
}
void br_init()
{
	//Baud rate setup
	BRG16 = 1; //Use 16-bit Baud rate 
	BRGH = 1; //High baud rate selected bit
	SPBRGH = 0x02; // SPBRG = 520 (BRG16 = 1; SYNC = 0; BRGH = 1, FOSC = 20MHz)
	SPBRG = 0x08; // Baud rate = 9600
}