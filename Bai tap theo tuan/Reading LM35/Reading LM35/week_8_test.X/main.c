
#include "mcc_generated_files/mcc.h"
#include "config.h"
#include <xc.h>
#include "lcd.h"
#include <stdio.h>

void main(void)
{
    ADC_Initialize();
    LCD_Initialize();
    float voltage;
    char s[8] = {0};
    while (1)
    {          
        //10mV = 1'C   
        ADC_TemperatureAcquisitionDelay();
        ADC_SelectChannel(channel_AN2);
        voltage = ((float)ADC_GetConversion(channel_AN2)*0.4887586-0.05); 
        sprintf(s, "Temp = %0.1f C", voltage);               
        LCDPutStr3("Hieu 20146127",0);
        LCDPutStr3(s,1);
        
        
    }
}