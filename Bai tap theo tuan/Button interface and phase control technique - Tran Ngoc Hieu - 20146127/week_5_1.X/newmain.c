// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
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

void main(void) {
    unsigned char state2 = 0, state1 = 0;
    unsigned int c = 0, d = 0, a = 0;
    TRISCbits.TRISC0 = 0; 
    TRISCbits.TRISC1 = 0; 
    TRISCbits.TRISC2 = 0; 
    TRISCbits.TRISC5 = 1;
    TRISCbits.TRISC6 = 1; 
    TRISCbits.TRISC7 = 1; 
    LATCbits.LATC0 = 0;
    LATCbits.LATC1 = 0; 
    LATCbits.LATC2 = 0; 

while(1){    

        //s1
        if ((state1==0)&&(PORTCbits.RC5==1)) {state1 = 1;a = 1;};
        if ((state1==1)&&(PORTCbits.RC5==0)) {state1 = 2;a = 1;};
        if ((state1==2)&&(PORTCbits.RC5==1)) {state1 = 3;a = 1;};
        if ((state1==3)&&(PORTCbits.RC5==0)) {state1 = 4;a = 1;};
        if ((state1==4)&&(PORTCbits.RC5==1)) {state1 = 5;a = 1;};
        if ((state1==5)&&(PORTCbits.RC5==0)) {state1 = 6;a = 1;};
        if ((state1==6)&&(PORTCbits.RC5==1)) {state1 = 7;a = 1;};
        if ((state1==7)&&(PORTCbits.RC5==0)) {state1 = 0;a = 1;};

        if (a == 1){
            state2=0;// khi nhan s1 thi trang thai s2 se duoc reset
            if ((state1==0)||(state1==1)){ 
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 0; 
              LATCbits.LATC2 = 0;
            }
            if ((state1==2)||(state1==3)){ 
              LATCbits.LATC0 = 1;
              LATCbits.LATC1 = 0; 
              LATCbits.LATC2 = 0;
            }
            if ((state1==4)||(state1==5)){ 
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 1; 
              LATCbits.LATC2 = 0;
            }
            if ((state1==6)||(state1==7)){ 
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 0; 
              LATCbits.LATC2 = 1;
            }
}

   
        
        //s2 
        if ((state2==0)&&(PORTCbits.RC6==1)) {state2 = 1;a = 2;};
        if ((state2==1)&&(PORTCbits.RC6==0)) {state2 = 2;a = 2;};
        if ((state2==2)&&(PORTCbits.RC6==1)) {state2 = 3;a = 2;};
        if ((state2==3)&&(PORTCbits.RC6==0)) {state2 = 4;a = 2;};
        if ((state2==4)&&(PORTCbits.RC6==1)) {state2 = 5;a = 2;};
        if ((state2==5)&&(PORTCbits.RC6==0)) {state2 = 6;a = 2;};
        if ((state2==6)&&(PORTCbits.RC6==1)) {state2 = 7;a = 2;};
        if ((state2==7)&&(PORTCbits.RC6==0)) {state2 = 0;a = 2;};
        
        
        if (a == 2){
            state1 = 0;// khi nhan s2 thi trang thai s1 se duoc reset
            if (state2==0){ 
                c = 0;
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 0; 
              LATCbits.LATC2 = 0;
            }  
          if ((state2==1)||(state2==2)) {
              LATCbits.LATC1 = 0; 
              LATCbits.LATC2 = 0;
              LATCbits.LATC0 = 1;
              if (c < 500) 
              {     c++;
                __delay_ms(1);}

              else if (c < 1000){
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 0; 
              LATCbits.LATC2 = 0;
              c++;
              __delay_ms(1);}

              else {
                  c = 0;
              }
            }

            if ((state2==3) || (state2==4)) {
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 1; 
              LATCbits.LATC2 = 0;
              if (c < 500) 
              {     c++;
                __delay_ms(1);}

              else if (c < 1000){
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 0; 
              LATCbits.LATC2 = 0;
              c++;
              __delay_ms(1);}

              else {
                  c = 0;
              }
            }

            if ((state2==5) || (state2==6)) {
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 0; 
              LATCbits.LATC2 = 1;
              if (c < 500) 
              {     c++;
                __delay_ms(1);}

              else if (c < 1000){
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 0; 
              LATCbits.LATC2 = 0;
              c++;
              __delay_ms(1);}

              else {
                  c = 0;
              }
            }

            if (state2==7) {
              LATCbits.LATC0 = 0;
              LATCbits.LATC1 = 0;
              LATCbits.LATC2 = 0; 
            }

}
        //s3 khi reset 3 con LED se sang len trong 0.5s roi tat
        if (PORTCbits.RC7 == 1) {
            if (d < 15) 
          {     d++;
            __delay_ms(200);}
            else if (PORTCbits.RC7 == 1){
              LATCbits.LATC0 = 1;
              LATCbits.LATC1 = 1;
              LATCbits.LATC2 = 1;
              __delay_ms(500);
                state1=0; 
                state2=0; 
                d = 0;
            }  
        }  
   }  
     
    while(1);       
    return;
}

