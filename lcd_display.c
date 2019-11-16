/*
 * lcd_display.c
 *
 *  Created on: Aug 7, 2019
 *      Author: gcrisler
 */

#include <msp430.h>

#include <driverlib.h>


#include "lcd_display.h"

const char top_row[10]={
                        0x77, //0
                        0x12, //1
                        0xE3, //2
                        0xD3, //3
                        0x96, //4
                        0xD5, //5
                        0xF5, //6
                        0x13, //7
                        0xF7, //8
                        0x97}; //9

const char middle_row[10]={
                        0xEE, //0
                        0xC0, //1
                        0x5E, //2
                        0xDA, //3
                        0xF0, //4
                        0xBA, //5
                        0xBE, //6
                        0xC8, //7
                        0xFE, //8
                        0xF8}; //9

 char buffer[10];
 unsigned int bcdreturn;
 unsigned int j,k;

 extern void LcdInit()
     {

     //Port select XT1
     GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,GPIO_PIN1 + GPIO_PIN2,GPIO_PRIMARY_MODULE_FUNCTION);

     //Set external frequency for XT1
     CS_setExternalClockSource(32768);

     //Select XT1 as the clock source for ACLK with no frequency divider
     CS_initClockSignal(CS_ACLK, CS_XT1CLK_SELECT, CS_CLOCK_DIVIDER_1);

     //Start XT1 with no time out
     CS_turnOnXT1(CS_XT1_DRIVE_0);

     //clear all OSC fault flag
     CS_clearAllOscFlagsWithTimeout(1000);

     /*
      * Disable the GPIO power-on default high-impedance mode to activate
      * previously configured port settings
      */
     PMM_unlockLPM5();

     // L0~L26 & L36~L39 pins selected
     LCD_E_setPinAsLCDFunctionEx(LCD_E_BASE, LCD_E_SEGMENT_LINE_0, LCD_E_SEGMENT_LINE_26);
     LCD_E_setPinAsLCDFunctionEx(LCD_E_BASE, LCD_E_SEGMENT_LINE_36, LCD_E_SEGMENT_LINE_39);

     LCD_E_initParam initParams = LCD_E_INIT_PARAM;
     initParams.clockDivider = LCD_E_CLOCKDIVIDER_8;
     initParams.muxRate = LCD_E_8_MUX;
     initParams.segments = LCD_E_SEGMENTS_ENABLED;

     // Init LCD as 4-mux mode
     LCD_E_init(LCD_E_BASE, &initParams);

     // LCD Operation - Mode 3, internal 3.08v, charge pump 256Hz
     LCD_E_setVLCDSource(LCD_E_BASE, LCD_E_INTERNAL_REFERENCE_VOLTAGE, LCD_E_EXTERNAL_SUPPLY_VOLTAGE);
     LCD_E_setVLCDVoltage(LCD_E_BASE, LCD_E_REFERENCE_VOLTAGE_3_08V);

     LCD_E_enableChargePump(LCD_E_BASE);
     LCD_E_setChargePumpFreq(LCD_E_BASE, LCD_E_CHARGEPUMP_FREQ_16);

     // Clear LCD memory
     LCD_E_clearAllMemory(LCD_E_BASE);

     // Configure COMs and SEGs
     // L0, L1, L2, L3: COM pins
     // L0 = COM0, L1 = COM1, L2 = COM2, L3 = COM3
     LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_0, LCD_E_MEMORY_COM0);
     LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_1, LCD_E_MEMORY_COM1);
     LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_2, LCD_E_MEMORY_COM2);
     LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_3, LCD_E_MEMORY_COM3);
     }

 void LcdDisplayBottomRow(long int incoming)
          {
           unsigned long int temp = 0;
           temp = Dec2BcdLong(incoming);
           LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_7, top_row[(temp & 0xf0000000) >>28]);
           LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_6, top_row[(temp & 0xf000000) >>24]);
           LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_5, top_row[(temp & 0xf00000) >>20]);
           LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_4, top_row[(temp & 0xf0000) >>16]);
           LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_3, top_row[(temp & 0xf000) >>12]);
           LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_2, top_row[(temp & 0x0f00) >>8]);
           LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_1, top_row[(temp & 0xf0) >>4]);
           LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_0, top_row[temp & 0x0f]);
           LCD_E_on(LCD_E_BASE);    // Turn LCD on
          }

 void LcdDisplayMiddleRow(long int incoming)
         {
          unsigned long int temp = 0;
          temp = Dec2BcdLong(incoming);
          LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_20, middle_row[(temp & 0xf0000000) >>28]);
          LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_19, middle_row[(temp & 0xf000000) >>24]);
          LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_9, middle_row[(temp & 0xf00000) >>20]);
          LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_10, middle_row[(temp & 0xf0000) >>16]);
          LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_11, middle_row[(temp & 0xf000) >>12]);
          LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_12, middle_row[(temp & 0x0f00) >>8]);
          LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_13, middle_row[(temp & 0xf0) >>4]);
          LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_14, middle_row[temp & 0x0f]);
          LCD_E_on(LCD_E_BASE); // Turn LCD on
         }

  void LcdDisplayTopRow(int incoming)
        {
         int temp = 0;
         temp = Dec2BCD(incoming);
         LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_18, top_row[(temp & 0xf000) >>12]);
         LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_17, top_row[(temp & 0x0f00) >>8]);
         LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_16, top_row[(temp & 0xf0) >>4]);
         LCD_E_setMemory(LCD_E_BASE, LCD_E_SEGMENT_LINE_15, top_row[temp & 0x0f]);
         LCD_E_on(LCD_E_BASE);  // Turn LCD on
        }

unsigned long int Dec2BcdLong(unsigned long int value)
      {
      unsigned int i;
      unsigned long int Output;
      for (i = 32, Output= 0; i; i--)// BCD Conversion,32-Bit
          {
           Output= __bcd_add_long(Output,Output);
           if (value& 0x80000000) Output= __bcd_add_long(Output,1);
           value<<= 1;
          }return Output;
      }

unsigned int Dec2BCD(int value)
     {
        unsigned int i;
        unsigned int Output;
        for (i = 16, Output = 0; i; i--) // BCD Conversion, 16-Bit
             {
             Output = __bcd_add_short(Output, Output);
             if (value & 0x8000) Output = __bcd_add_short(Output, 1);
             value <<= 1;
             } return Output;
     }

extern char* itoa(int value, char* result)
    {

            char* ptr = result, *ptr1 = result, tmp_char;
            int tmp_value;

            do {
                tmp_value = value;
                value /= 10;
                *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * 10)];
                } while ( value );

    // Apply negative sign

        *ptr-- = '\0';
        while(ptr1 < ptr)
            {
            tmp_char = *ptr;
            *ptr--= *ptr1;
            *ptr1++ = tmp_char;
            }
        return result;
    }



