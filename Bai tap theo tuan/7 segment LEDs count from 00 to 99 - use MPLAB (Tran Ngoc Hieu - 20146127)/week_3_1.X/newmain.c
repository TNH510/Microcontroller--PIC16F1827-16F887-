/*
 * File:   newmain.c
 * Author: Hieu
 *
 * Created on February 23, 2022, 9:25 PM
 */
// CONFIG1
#pragma config FOSC = INTOSC   // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

#include <xc.h>
#define _XTAL_FREQ 500000

int A[10] = {64,121,36,48,25,18,2,120,0,16};
int B[10] = {0,4,32,0,4,16,16,0,0,0};
int C[10] = {128,240,64,96,48,32,0,240,0,32};
signed int i ;
signed int j;
void main(void) {
    
    TRISC = 0;
    TRISA = 0;
    TRISB = 0;    
    j = 0;
    LATC = 64;
    while(j < 10)
    {   
        LATA = B[j];
        LATB = C[j];
        j = j+1;
        i = 0;
        while(i < 10)
        {
            LATC = A[i];
            __delay_ms(250);
            i = i +1;        
        }      
    }
    return;
}
