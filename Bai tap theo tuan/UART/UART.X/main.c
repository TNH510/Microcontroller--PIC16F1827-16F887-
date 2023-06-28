#include "mcc_generated_files/mcc.h"

void putch(char data) {
    while(!EUSART_is_tx_ready());
    EUSART_Write(data);}
unsigned char CMD[10] = {0,0,0,0,0,0,0,0,0,0};
unsigned char N[10] = {'0','1','2','3','4','5','6','7','8','9'};

void led_turn_on(int LED_number)
{
    if (LED_number == 0) LATCbits.LATC0 = 1;
    if (LED_number == 1) LATCbits.LATC1 = 1;
    if (LED_number == 2) LATCbits.LATC2 = 1;
    if (LED_number == 3) LATCbits.LATC3 = 1;
    if (LED_number == 4) LATCbits.LATC4 = 1;
    if (LED_number == 5) LATCbits.LATC5 = 1;
    if (LED_number == 6) LATCbits.LATC6 = 1;
    if (LED_number == 7) LATCbits.LATC7 = 1;
}
void led_turn_off(int LED_number)
{
    if (LED_number == 0) LATCbits.LATC0 = 0;
    if (LED_number == 1) LATCbits.LATC1 = 0;
    if (LED_number == 2) LATCbits.LATC2 = 0;
    if (LED_number == 3) LATCbits.LATC3 = 0;
    if (LED_number == 4) LATCbits.LATC4 = 0;
    if (LED_number == 5) LATCbits.LATC5 = 0;
    if (LED_number == 6) LATCbits.LATC6 = 0;
    if (LED_number == 7) LATCbits.LATC7 = 0;
}



void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    while (1)
    {
         
        for (int i=0; i <8; i++)
        {   
            while(EUSART_is_tx_ready()==0)  ;   
            CMD[i] = EUSART_Read();
            if (CMD[i] == '#') {i = 8;}
        }     
        
        //Turn on LED
        for (int i=0; i<8; i++)
        {
            if ((CMD[0] == 'O')&&(CMD[1] == 'N')&&(CMD[2] == '0')&&(CMD[3] == N[i])&&(CMD[4] == '#'))            
            { led_turn_on(i);}
            if ((CMD[0] == 'O')&&(CMD[1] == 'N')&&(CMD[2] == '*')&&(CMD[3] == '#'))
            {LATC = 0b11111111;}
            if ((CMD[0] == 'O')&&(CMD[1] == 'F')&&(CMD[2] == 'F')&&(CMD[3] == '0')&&(CMD[4] == N[i])&&(CMD[5] == '#'))            
            { led_turn_off(i);}
            if ((CMD[0] == 'O')&&(CMD[1] == 'F')&&(CMD[2] == 'F')&&(CMD[3] == '*')&&(CMD[4] == '#'))
            {LATC = 0b00000000;}
             
            if ((CMD[0] == 'B')&&(CMD[1] == 'L')&&(CMD[2] == 'I')&&(CMD[3] == 'N')&&(CMD[4] == 'K')&&(CMD[5] == '*')&&(CMD[6] == '#'))
            {
                
            LATC = 0b11111111;
            __delay_ms(500);
            LATC = 0b00000000;
            __delay_ms(500);
             while(EUSART_is_tx_ready()==0)   
             {LATC = 0b11111111;
            __delay_ms(500);
            LATC = 0b00000000;
            __delay_ms(500);}
            }
            if ((CMD[0] == 'B')&&(CMD[1] == 'L')&&(CMD[2] == 'I')&&(CMD[3] == 'N')&&(CMD[4] == 'K')&&(CMD[5] == '0')&&(CMD[6] == N[i])&&(CMD[7] == '#'))            
            {   

                led_turn_on(i);
               __delay_ms(500);
               led_turn_off(i);
               __delay_ms(500);
               led_turn_on(i);
               __delay_ms(500);
               led_turn_off(i);
               __delay_ms(500);
               led_turn_on(i);
               __delay_ms(500);
               led_turn_off(i);
               __delay_ms(500);


             }
            
        }  
        
        
        
        

    }
}
