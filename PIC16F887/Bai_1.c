#include<htc.h>
#include<stdio.h>
#include<stdlib.h>
#include "lcd(16).h"

#define _XTAL_FREQ 4000000

__CONFIG(FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);
//FOSC_INTRC_NOCLKOUT
//OSCCON = 0b01100001;
void gocquay (int goc);
int A,dem,i;
char a[5];
char goc[5];
int angle;
void putch(char c);
void tx_init();
void rx_init();
void br_init();
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

void main() {
	lcd_init();

	//Set internal clock to 2MHz. Need to set this first, then setup timer. //Used for setup baud-rate
	IRCF2 = 1; IRCF1 = 1; IRCF0 = 0; //Setup FOSC = 1MHz
	SCS = 1; // Internal oscillator is used for system clock

	ANSEL = ANSELH = 0;  // diasble analog all I/O
	TRISE0 = 0; RE0 = 1;
	//timer 2 set up
	T2CKPS1 = 0; T2CKPS1 = 0; // 1:1

	PR2 = 99; TMR2 = 0;//dinh thoi 20uS
	TMR2ON = 1; // on timer 2
	TMR2IE = 1; 
	TMR2IF = 0;
 	PEIE = 1; GIE =1;
	tx_init();
	rx_init();
	br_init();
	
while(1)
	{
	
	for (int dem = 0;dem < 3;dem++)
	{
		goc[dem] = a[dem];
	}
	angle = atoi(goc);
	gocquay(angle);
	lcd_gotoxy(0,1);
	printf("%03u",angle);
	}	

}
void gocquay (int goc)
{	

	A = 10+goc*(1/18.0);
	
}
void interrupt ngat()
{
	if(TMR2IF)
	{
		dem++;
		if(dem == A) {RE0 = 0;}
		else if(dem ==200){dem =0;RE0 = 1;}
	}
	TMR2IF = 0;
	
	if(RCIF)
	{
		a[i] = RCREG;
		i++;
		if(i == 3) i = 0;
	}
	RCIF = 0;
}

void putch(char c)
{
	lcd_putc(c);
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
	SPBRGH = 0b0000000; // SPBRG = 103
	SPBRG = 0b01100111; // Baud rate = 9600
}