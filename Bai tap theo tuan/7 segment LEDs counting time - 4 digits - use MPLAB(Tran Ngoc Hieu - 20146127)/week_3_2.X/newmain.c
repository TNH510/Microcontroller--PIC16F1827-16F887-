/*
 * File:   newmain.c
 * Author: Hieu
 *
 * Created on Ngày 01 tháng 3 n?m 2022, 10:03
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#define _XTAL_FREQ 500000

int A[10] = {63,6,91,79,102,109,125,7,127,111};
int B[10] = {63,6,91,79,102,109,125,7,127,111};
int C[10] = {127,70,91,79,102,109,125,103,127,111};
int D[10] = {126,126,127,127,127,127,127,126,127,127};
int E[10] = {63,6,91,79,102,109,125,7,127,111};
signed int i;
signed int j;
signed int k;
signed int l;

void main(void) {
    
    TRISC = 0;
    TRISD = 0;
    TRISA = 0;
    TRISE = 0;
    TRISB = 0;
    
    l = 0;
    while(l < 6)
    {
        PORTB = E[l];       
        k = 0;
        while(k<10)
        {
            PORTA = C[k];
            PORTE = D[k];
            j = 0;
            while(j < 6)
            {    
                PORTD = B[j];
                i = 0;
                while(i < 10)
                {
                    PORTC = A[i];
                    i = i +1;
                    __delay_ms(100);
                }
                j = j +1;
            }
            k = k +1;
        }
        l = l +1;
    }
    return;
}
