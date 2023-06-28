#include<htc.h>
#include<stdio.h>
#include<stdlib.h>
#include "lcd(16).h"

#define _XTAL_FREQ 20000000

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);
//FOSC_INTRC_NOCLKOUT
//OSCCON = 0b01100001;
void TX();
int dem,xung,pwm_t;
float v;
char data;
char data_rx[12];
char data_pwm[3];
int b;
int a = 0;
void pwm_10bit(int pwm)
{
	CCPR1L  = pwm/4;
	DC1B1 = 0; DC1B0 = 0;
	if((pwm & 2) == 1) DC1B1 = 1;
	if((pwm & 1) == 1) DC1B0 = 1;
}
void TX()
{
	SYNC = 0; BRGH = 1; BRG16 = 1;
	SPBRGH = 0x02; // 9600
	SPBRG = 0x08; 
	TXEN = 1; TX9 = 0; SPEN = 1;
}

void RX()
{
	SYNC = 0; BRGH = 1; BRG16 = 1;
	SPBRGH = 0x02; // 9600
	SPBRG = 0x08; 
	CREN = 1; RX9 = 0; SPEN = 1;
}


void send_char(char data)
{

		while(!TXIF);
		TXREG = data;


}


void send_string(const char *s)
{
	while(*s) {send_char(*s++);}

}




void main()
{	

 	ANSEL = ANSELH = 0;  // diasble analog all I/O
	lcd_init();
	TRISE = 0; RE0 = 0; RE1 = 0; RE2 = 0;
	///////////////
	TRISC2 = 0;
	//timer 2 set up
	T2CKPS1 = 0; T2CKPS0 = 1; // 1:4
	PR2 = 124; TMR2 = 0;
	TMR2ON = 1; // on timer 2
 /// CCP1CON set up pum mode
	CCP1CON = 0b00001100;
//////Timer 1 set up - counter mode
	TMR1CS = 1;
	T1CKPS1 = 0;T1CKPS0 = 0;  //1:1
	T1SYNC  = 1;
	TMR1 = 0;
	TMR1GE = 0; TMR1ON = 1;
///////////////////////////
//////Timer 0 set up - timer mode - 100ms
	T0CS = 0;
	PSA  = 0; // cap bo chia
	PS2 = 0; PS1 = 1;PS0 = 0;
    TMR0 = 6;
	T0IE = 1; T0IF = 0; GIE = 1; PEIE =1;
////////////////////////////////
	pwm_t = 400;
	pwm_10bit(pwm_t);

//////////////////////////////////////
	TX();
	RX();
	while(1)
	{	

		for(b = 2; b< 5; b++)
		{data_pwm[b - 2] = data_rx[b];}
		
		
		send_string(data_pwm);
		pwm_t = atoi(data_pwm);
		
		v = (xung/374.0)*10.0*60.0;
	//	if(v < 100.0)pwm_t = pwm_t++;
	//	else if(v > 100.0)pwm_t = pwm_t--;

		pwm_10bit(pwm_t);
		lcd_gotoxy(0,0);
		printf("v: %3.2f",v);
		lcd_gotoxy(0,1);
		printf("xung: %05d",xung);
	}	
}
void interrupt ngat()
{
	if(T0IF)
	{	TMR0 = 6;
		dem++;
		if(dem == 250)
			{ 
				dem = 0;
				xung = TMR1;
				TMR1 = 0; 
			}
	}
	T0IF = 0;


	if(RCIF)
	{	

		data_rx[a] = RCREG;
		a++; if (a == 5) a = 0;
		if (RCREG == 't') { RE1 = 1; RE2 = 0; TMR2ON = 1;}
		else if (RCREG == 'd') {RE1 = 0; RE2 = 0; TMR2ON = 0;}
		else if (RCREG == 'p') {RE1 = 0; RE2 = 1; TMR2ON = 1;}
		else if (RCREG == 'r') {a = 0;}

		
	}
	RCIF = 0;
}

void putch(char c)
{
	lcd_putc(c);
}