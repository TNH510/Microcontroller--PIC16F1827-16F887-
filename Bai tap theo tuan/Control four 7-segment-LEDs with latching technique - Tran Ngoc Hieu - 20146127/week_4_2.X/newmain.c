// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
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

int A[80] = {
    0,0,0,0,0,0,1,1,  1,0,0,1,1,1,1,1,  0,0,1,0,0,1,0,1,
    0,0,0,0,1,1,0,1,  1,0,0,1,1,0,0,1,  0,1,0,0,1,0,0,1,
    0,1,0,0,0,0,0,1,  0,0,0,1,1,1,1,1,  0,0,0,0,0,0,0,1,  
    0,0,0,0,1,0,0,1,                                        };   
void data(unsigned char a){
    signed int i;
    for(i=8*a+7;i>=8*a;i--)
        {   
            LATCbits.LATC1 = A[i];
            LATCbits.LATC0 = 1;
            LATCbits.LATC0 = 0;
        }       }
void hienthi(unsigned char a,unsigned char b,unsigned char c,unsigned char d){
        data(d); data(c); data(b); data(a);
        LATCbits.LATC2 = 1;
        LATCbits.LATC2 = 0;    }
void main(void) {
    signed int a; signed int b; signed int c; signed int d;
    
        TRISCbits.TRISC0 = 0; //CLOCK
        TRISCbits.TRISC1 = 0; //DATA
        TRISCbits.TRISC2 = 0; //LATCH
        LATCbits.LATC0 = 0; 
        LATCbits.LATC1 = 0;
        LATCbits.LATC2 = 0;       
        a = 5;
        while(a>=0)
        {
            b = 9;
            while(b>=0)
            {
                c = 5;
                while(c>=0)
                {
                    d = 9;
                    while(d>=0)
                    {   
                            hienthi(a,b,c,d);
                            __delay_ms(100);
                            d = d-1;    
                    }   c = c-1;
                }   b = b-1;
            }   a = a-1;
        }
    return;
}
