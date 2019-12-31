/*
 * lcd_display.c
 *
 *  Created on: Aug 7, 2019
 *      Author: gcrisler
 */

#include <msp430.h>

#include <driverlib.h>


#include "lcd_display.h"


 char buffer[10];
 unsigned int bcdreturn;
 unsigned int j,k;

 extern void LcdInit()
     {

     }

 const unsigned int segment_data[10]=
                         {
                          0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x7B
                         };

 /*const unsigned int segment_data[10]=
                         {0x3F,// display 0
                         0x06,  // display 1
                         0x5B,
                         0x4F,  //display 3
                         0x66,  //display 4
                         0x6D,  //display 5
                         0x7D,  //display 6
                         0x07,  //display 7
                         0x7F,  //display 8
                         0x67   //display 9
                        };
*/
 const unsigned int lcd_data[4][14]=
              {7,0x10,7,0x08,7,0x04,7,0x02,6,0x04,6,0x10,6,0x08, // data for digit 1
               9,0x10,9,0x08,9,0x04,8,0x02,8,0x04,8,0x10,8,0x08,
               10,0x10,11,0x10,11,0x04,10,0x02,9,0x02,10,0x08,10,0x04,
               12,0x10,12,0x08,12,0x02,11,0x01,11,0x02,11,0x08,12,0x04
              };

 void LcdClearTopRow(void)
         {
             LCDM6W &= 0x0001;
             LCDM8W &= 0x0101;
             LCDM10W &= 0x0001;
             LCDM12W &= 0x0001;
         }

 void LcdDisplayBottomRow(unsigned long int incoming)
          {
            unsigned long int temp;
            unsigned int lsb7;
            unsigned int lsb6;
            unsigned int lsb5;
            unsigned int lsb4;
            unsigned int lsb3;
            unsigned int lsb2;
            unsigned int lsb1;
            unsigned int lsb0;

            temp = Dec2BcdLong(incoming);
            lsb7 = (temp & 0xF0000000)>>28;
            lsb6 = (temp & 0xF000000)>>24;
            lsb5 = (temp & 0xF00000)>>20;
            lsb4 = (temp & 0xF0000)>>16;
            lsb3 = (temp & 0xF000)>>12;
            lsb2 = (temp & 0xF00)>>8;
            lsb1 = (temp & 0xF0)>>4;
            lsb0 = (temp & 0x0F)>>0;

            LCDM37 &= 0x07;LCDM36 &= 0x03;LCDM35 &= 0x07; //clear out bits that are related to digit 13
            switch(lsb7)
             {
             case 0:LCDM37 |= 0x18;LCDM36 |= 0x14;LCDM35 |= 0x18;break;
             case 1:LCDM37 |= 0x00;LCDM36 |= 0x00;LCDM35 |= 0x18;break;
             case 2:LCDM37 |= 0x10;LCDM36 |= 0x1C;LCDM35 |= 0x08;break;
             case 3:LCDM37 |= 0x00;LCDM36 |= 0x1C;LCDM35 |= 0x18;break;
             case 4:LCDM37 |= 0x08;LCDM36 |= 0x08;LCDM35 |= 0x18;break;
             case 5:LCDM37 |= 0x08;LCDM36 |= 0x1C;LCDM35 |= 0x10;break;
             case 6:LCDM37 |= 0x18;LCDM36 |= 0x1C;LCDM35 |= 0x10;break;
             case 7:LCDM37 |= 0x00;LCDM36 |= 0x04;LCDM35 |= 0x18;break;
             case 8:LCDM37 |= 0x18;LCDM36 |= 0x1C;LCDM35 |= 0x18;break;
             case 9:LCDM37 |= 0x08;LCDM36 |= 0x1C;LCDM35 |= 0x18;break;
            }

            LCDM34 &= 0x07;LCDM33 &= 0x03;LCDM32 &= 0x013;//clear out bits that are related to digit 14
            switch(lsb6)
              {
              case 0:LCDM34 |= 0x18;LCDM33 |= 0x14;LCDM32 |= 0x0C;break;
              case 1:LCDM34 |= 0x00;LCDM33 |= 0x00;LCDM32 |= 0x0C;break;
              case 2:LCDM34 |= 0x10;LCDM33 |= 0x1C;LCDM32 |= 0x04;break;
              case 3:LCDM34 |= 0x00;LCDM33 |= 0x1C;LCDM32 |= 0x0C;break;
              case 4:LCDM34 |= 0x08;LCDM33 |= 0x08;LCDM32 |= 0x0C;break;
              case 5:LCDM34 |= 0x08;LCDM33 |= 0x1C;LCDM32 |= 0x08;break;
              case 6:LCDM34 |= 0x18;LCDM33 |= 0x1C;LCDM32 |= 0x08;break;
              case 7:LCDM34 |= 0x00;LCDM33 |= 0x04;LCDM32 |= 0x0C;break;
              case 8:LCDM34 |= 0x18;LCDM33 |= 0x1C;LCDM32 |= 0x0C;break;
              case 9:LCDM34 |= 0x08;LCDM33 |= 0x1C;LCDM32 |= 0x0C;break;
             }

            LCDM31 &= 0x07;LCDM30 &= 0x03;LCDM29 &=0x07;    //clear out bits that are related to digit15

            switch(lsb5)
               {
               case 0:LCDM31 |= 0x18;LCDM30 |= 0x14;LCDM29 |= 0x18;break;
               case 1:LCDM31 |= 0x00;LCDM30 |= 0x00;LCDM29 |= 0x18;break;
               case 2:LCDM31 |= 0x10;LCDM30 |= 0x1C;LCDM29 |= 0x08;break;
               case 3:LCDM31 |= 0x00;LCDM30 |= 0x1C;LCDM29 |= 0x18;break;
               case 4:LCDM31 |= 0x08;LCDM30 |= 0x08;LCDM29 |= 0x18;break;
               case 5:LCDM31 |= 0x08;LCDM30 |= 0x1C;LCDM29 |= 0x10;break;
               case 6:LCDM31 |= 0x18;LCDM30 |= 0x1C;LCDM29 |= 0x10;break;
               case 7:LCDM31 |= 0x00;LCDM30 |= 0x04;LCDM29 |= 0x18;break;
               case 8:LCDM31 |= 0x18;LCDM30 |= 0x1C;LCDM29 |= 0x18;break;
               case 9:LCDM31 |= 0x08;LCDM30 |= 0x1C;LCDM29 |= 0x18;break;
              }

        LCDM28 &= 0x07;LCDM27 &= 0x03;LCDM26 &= 0x013; //clear out bits that are related to digit 16

         switch(lsb4)
            {
            case 0:LCDM28 |= 0x18;LCDM27 |= 0x14;LCDM26 |= 0x0C;break;
            case 1:LCDM28 |= 0x00;LCDM27 |= 0x00;LCDM26 |= 0x0C;break;
            case 2:LCDM28 |= 0x10;LCDM27 |= 0x1C;LCDM26 |= 0x04;break;
            case 3:LCDM28 |= 0x00;LCDM27 |= 0x1C;LCDM26 |= 0x0C;break;
            case 4:LCDM28 |= 0x08;LCDM27 |= 0x08;LCDM26 |= 0x0C;break;
            case 5:LCDM28 |= 0x08;LCDM27 |= 0x1C;LCDM26 |= 0x08;break;
            case 6:LCDM28 |= 0x18;LCDM27 |= 0x1C;LCDM26 |= 0x08;break;
            case 7:LCDM28 |= 0x00;LCDM27 |= 0x04;LCDM26 |= 0x0C;break;
            case 8:LCDM28 |= 0x18;LCDM27 |= 0x1C;LCDM26 |= 0x0C;break;
            case 9:LCDM28 |= 0x08;LCDM27 |= 0x1C;LCDM26 |= 0x0C;break;
           }

         LCDM25 &= 0x07;LCDM24 &= 0x03;LCDM23 &= 0x013;  //clear out bits that are related to digit 17

        switch(lsb3)
           {
           case 0:LCDM25 |= 0x18;LCDM24 |= 0x14;LCDM23 |= 0x0C;break;
           case 1:LCDM25 |= 0x00;LCDM24 |= 0x00;LCDM23 |= 0x0C;break;
           case 2:LCDM25 |= 0x10;LCDM24 |= 0x1C;LCDM23 |= 0x04;break;
           case 3:LCDM25 |= 0x00;LCDM24 |= 0x1C;LCDM23 |= 0x0C;break;
           case 4:LCDM25 |= 0x08;LCDM24 |= 0x08;LCDM23 |= 0x0C;break;
           case 5:LCDM25 |= 0x08;LCDM24 |= 0x1C;LCDM23 |= 0x08;break;
           case 6:LCDM25 |= 0x18;LCDM24 |= 0x1C;LCDM23 |= 0x08;break;
           case 7:LCDM25 |= 0x00;LCDM24 |= 0x04;LCDM23 |= 0x0C;break;
           case 8:LCDM25 |= 0x18;LCDM24 |= 0x1C;LCDM23 |= 0x0C;break;
           case 9:LCDM25 |= 0x08;LCDM24 |= 0x1C;LCDM23 |= 0x0C;break;
          }

            LCDM22 &= 0x07;LCDM21 &= 0x03; LCDM20 &= 0x07;//clear out bits that are related to digit18

        switch(lsb2)
           {
           case 0:LCDM22 |= 0x18;LCDM21 |= 0x14;LCDM20 |= 0x18;break;
           case 1:LCDM22 |= 0x00;LCDM21 |= 0x00;LCDM20 |= 0x18;break;
           case 2:LCDM22 |= 0x10;LCDM21 |= 0x1C;LCDM20 |= 0x08;break;
           case 3:LCDM22 |= 0x00;LCDM21 |= 0x1C;LCDM20 |= 0x18;break;
           case 4:LCDM22 |= 0x08;LCDM21 |= 0x08;LCDM20 |= 0x18;break;
           case 5:LCDM22 |= 0x08;LCDM21 |= 0x1C;LCDM20 |= 0x10;break;
           case 6:LCDM22 |= 0x18;LCDM21 |= 0x1C;LCDM20 |= 0x10;break;
           case 7:LCDM22 |= 0x00;LCDM21 |= 0x04;LCDM20 |= 0x18;break;
           case 8:LCDM22 |= 0x18;LCDM21 |= 0x1C;LCDM20 |= 0x18;break;
           case 9:LCDM22 |= 0x08;LCDM21 |= 0x1C;LCDM20 |= 0x18;break;
          }


        LCDM18 &= 0x07;LCDM17 &= 0x03;LCDM16 &= 0x07;//clear out bits that are related to digit19

       switch(lsb1)
          {
          case 0:LCDM18 |= 0x18;LCDM17 |= 0x14;LCDM16 |= 0x18;break;
          case 1:LCDM18 |= 0x00;LCDM17 |= 0x00;LCDM16 |= 0x18;break;
          case 2:LCDM18 |= 0x10;LCDM17 |= 0x1C;LCDM16 |= 0x08;break;
          case 3:LCDM18 |= 0x00;LCDM17 |= 0x1C;LCDM16 |= 0x18;break;
          case 4:LCDM18 |= 0x08;LCDM17 |= 0x08;LCDM16 |= 0x18;break;
          case 5:LCDM18 |= 0x08;LCDM17 |= 0x1C;LCDM16 |= 0x10;break;
          case 6:LCDM18 |= 0x18;LCDM17 |= 0x1C;LCDM16 |= 0x10;break;
          case 7:LCDM18 |= 0x00;LCDM17 |= 0x04;LCDM16 |= 0x18;break;
          case 8:LCDM18 |= 0x18;LCDM17 |= 0x1C;LCDM16 |= 0x18;break;
          case 9:LCDM18 |= 0x08;LCDM17 |= 0x1C;LCDM16 |= 0x18;break;
         }

       LCDM14 &= 0x03;LCDM13 &= 0x03;    //clear out bits that are related to digit19

       switch(lsb0)
        {
        case 0:LCDM14 |= 0x1C;LCDM13 |= 0x1A;break;
        case 1:LCDM14 |= 0x00;LCDM13 |= 0x0A;break;
        case 2:LCDM14 |= 0x14;LCDM13 |= 0x16;break;
        case 3:LCDM14 |= 0x04;LCDM13 |= 0x1E;break;
        case 4:LCDM14 |= 0x08;LCDM13 |= 0x0E;break;
        case 5:LCDM14 |= 0x0C;LCDM13 |= 0x1C;break;
        case 6:LCDM14 |= 0x1C;LCDM13 |= 0x1C;break;
        case 7:LCDM14 |= 0x04;LCDM13 |= 0x0A;break;
        case 8:LCDM14 |= 0x1C;LCDM13 |= 0x1E;break;
        case 9:LCDM14 |= 0x0C;LCDM13 |= 0x1E;break;
       }

          }

 void LcdDisplayMiddleRow(unsigned long int incoming)
         {
           unsigned long int temp;
           unsigned int lsb7;
           unsigned int lsb6;
           unsigned int lsb5;
           unsigned int lsb4;
           unsigned int lsb3;
           unsigned int lsb2;
           unsigned int lsb1;
           unsigned int lsb0;

           temp = Dec2BcdLong(incoming);
           lsb7 = (temp & 0xF0000000)>>28;
           lsb6 = (temp & 0xF000000)>>24;
           lsb5 = (temp & 0xF00000)>>20;
           lsb4 = (temp & 0xF0000)>>16;
           lsb3 = (temp & 0xF000)>>12;
           lsb2 = (temp & 0xF00)>>8;
           lsb1 = (temp & 0xF0)>>4;
           lsb0 = (temp & 0x0F)>>0;

           LCDM37 &= 0x18;LCDM36 &= 0x1C;LCDM35 &= 0x1C;   //clear out bits that are related to msb digi 5

       switch(lsb7)
           {
           case 0:LCDM35 |= 0x03;LCDM36W |= 0x0702;break;
           case 1:LCDM35 |= 0x03;LCDM36W |= 0x0000;break;
           case 2:LCDM35 |= 0x01;LCDM36W |= 0x0503;break;
           case 3:LCDM35 |= 0x03;LCDM36W |= 0x0103;break;
           case 4:LCDM35 |= 0x03;LCDM36W |= 0x0201;break;
           case 5:LCDM35 |= 0x02;LCDM36W |= 0x0303;break;
           case 6:LCDM35 |= 0x02;LCDM36W |= 0x0703;break;
           case 7:LCDM35 |= 0x03;LCDM36W |= 0x0100;break;
           case 8:LCDM35 |= 0x03;LCDM36W |= 0x0703;break;
           case 9:LCDM35 |= 0x03;LCDM36W |= 0x0303;break;
          }

       LCDM34 &= 0x18;LCDM33 &= 0x1C;LCDM32 &= 0x1C;    //clear out bits that are related to digit6

       switch(lsb6)
          {
          case 0:LCDM32 |= 0x03;LCDM33 |= 0x02;LCDM34 |= 0x07;break;
          case 1:LCDM32 |= 0x03;LCDM33 |= 0x00;LCDM34 |= 0x00;break;
          case 2:LCDM32 |= 0x01;LCDM33 |= 0x03;LCDM34 |= 0x05;break;
          case 3:LCDM32 |= 0x03;LCDM33 |= 0x03;LCDM34 |= 0x01;break;
          case 4:LCDM32 |= 0x03;LCDM33 |= 0x01;LCDM34 |= 0x02;break;
          case 5:LCDM32 |= 0x02;LCDM33 |= 0x03;LCDM34 |= 0x03;break;
          case 6:LCDM32 |= 0x02;LCDM33 |= 0x03;LCDM34 |= 0x07;break;
          case 7:LCDM32 |= 0x03;LCDM33 |= 0x00;LCDM34 |= 0x01;break;
          case 8:LCDM32 |= 0x03;LCDM33 |= 0x03;LCDM34 |= 0x07;break;
          case 9:LCDM32 |= 0x03;LCDM33 |= 0x03;LCDM34 |= 0x03;break;
         }

         LCDM31 &= 0x18;LCDM30 &= 0x1C;LCDM29 &= 0x1C;    //clear out bits that are related to digi7

        switch(lsb5)
           {
           case 0:LCDM29 |= 0x03;LCDM30 |= 0x02;LCDM31 |= 0x07;break;
           case 1:LCDM29 |= 0x03;LCDM30 |= 0x00;LCDM31 |= 0x00;break;
           case 2:LCDM29 |= 0x01;LCDM30 |= 0x03;LCDM31 |= 0x05;break;
           case 3:LCDM29 |= 0x03;LCDM30 |= 0x03;LCDM31 |= 0x01;break;
           case 4:LCDM29 |= 0x03;LCDM30 |= 0x01;LCDM31 |= 0x02;break;
           case 5:LCDM29 |= 0x02;LCDM30 |= 0x03;LCDM31 |= 0x03;break;
           case 6:LCDM29 |= 0x02;LCDM30 |= 0x03;LCDM31 |= 0x07;break;
           case 7:LCDM29 |= 0x03;LCDM30 |= 0x00;LCDM31 |= 0x01;break;
           case 8:LCDM29 |= 0x03;LCDM30 |= 0x03;LCDM31 |= 0x07;break;
           case 9:LCDM29 |= 0x03;LCDM30 |= 0x03;LCDM31 |= 0x03;break;
          }
          LCDM28 &= 0x18;LCDM27 &= 0x1C;LCDM26 &= 0x1C;    //clear out bits that are related to digi8

       switch(lsb4)
          {
          case 0:LCDM26 |= 0x03;LCDM27 |= 0x02;LCDM28 |= 0x07;break;
          case 1:LCDM26 |= 0x03;LCDM27 |= 0x00;LCDM28 |= 0x00;break;
          case 2:LCDM26 |= 0x01;LCDM27 |= 0x03;LCDM28 |= 0x05;break;
          case 3:LCDM26 |= 0x03;LCDM27 |= 0x03;LCDM28 |= 0x01;break;
          case 4:LCDM26 |= 0x03;LCDM27 |= 0x01;LCDM28 |= 0x02;break;
          case 5:LCDM26 |= 0x02;LCDM27 |= 0x03;LCDM28 |= 0x03;break;
          case 6:LCDM26 |= 0x02;LCDM27 |= 0x03;LCDM28 |= 0x07;break;
          case 7:LCDM26 |= 0x03;LCDM27 |= 0x00;LCDM28 |= 0x01;break;
          case 8:LCDM26 |= 0x03;LCDM27 |= 0x03;LCDM28 |= 0x07;break;
          case 9:LCDM26 |= 0x03;LCDM27 |= 0x03;LCDM28 |= 0x03;break;
         }

         LCDM25 &= 0x18;LCDM24 &= 0x1C;LCDM23 &= 0x1C;    //clear out bits that are related to digi9

       switch(lsb3)
          {
          case 0:LCDM23 |= 0x03;LCDM24 |= 0x02;LCDM25 |= 0x07;break;
          case 1:LCDM23 |= 0x03;LCDM24 |= 0x00;LCDM25 |= 0x00;break;
          case 2:LCDM23 |= 0x01;LCDM24 |= 0x03;LCDM25 |= 0x05;break;
          case 3:LCDM23 |= 0x03;LCDM24 |= 0x03;LCDM25 |= 0x01;break;
          case 4:LCDM23 |= 0x03;LCDM24 |= 0x01;LCDM25 |= 0x02;break;
          case 5:LCDM23 |= 0x02;LCDM24 |= 0x03;LCDM25 |= 0x03;break;
          case 6:LCDM23 |= 0x02;LCDM24 |= 0x03;LCDM25 |= 0x07;break;
          case 7:LCDM23 |= 0x03;LCDM24 |= 0x00;LCDM25 |= 0x01;break;
          case 8:LCDM23 |= 0x03;LCDM24 |= 0x03;LCDM25 |= 0x07;break;
          case 9:LCDM23 |= 0x03;LCDM24 |= 0x03;LCDM25 |= 0x03;break;
         }


        LCDM22 &= 0x18;LCDM21 &= 0x1C; LCDM20 &= 0x1C; //clear out bits that are related to digi10

      switch(lsb2)
         {
         case 0:LCDM20 |= 0x03;LCDM21|= 0x02;LCDM22 |= 0x07;break;
         case 1:LCDM20 |= 0x03;LCDM21|= 0x00;LCDM22 |= 0x00;break;
         case 2:LCDM20 |= 0x01;LCDM21|= 0x03;LCDM22 |= 0x05;break;
         case 3:LCDM20 |= 0x03;LCDM21|= 0x03;LCDM22 |= 0x01;break;
         case 4:LCDM20 |= 0x03;LCDM21|= 0x01;LCDM22 |= 0x02;break;
         case 5:LCDM20 |= 0x02;LCDM21|= 0x03;LCDM22 |= 0x03;break;
         case 6:LCDM20 |= 0x02;LCDM21|= 0x03;LCDM22 |= 0x07;break;
         case 7:LCDM20 |= 0x03;LCDM21|= 0x00;LCDM22 |= 0x01;break;
         case 8:LCDM20 |= 0x03;LCDM21|= 0x03;LCDM22 |= 0x07;break;
         case 9:LCDM20 |= 0x03;LCDM21|= 0x03;LCDM22 |= 0x03;break;
        }

       LCDM19&= 0x1C;LCDM18 &= 0x1B;LCDM17 &= 0x1C;    //clear out bits that are related to digi11

     switch(lsb1)
        {
         case 0:LCDM19 |= 0x03;LCDM18 |= 0x05;LCDM17 |= 0x03;break;
         case 1:LCDM19 |= 0x00;LCDM18 |= 0x00;LCDM17 |= 0x03;break;
         case 2:LCDM19 |= 0x02;LCDM18 |= 0x07;LCDM17 |= 0x01;break;
         case 3:LCDM19 |= 0x00;LCDM18 |= 0x07;LCDM17 |= 0x03;break;
         case 4:LCDM19 |= 0x01;LCDM18 |= 0x02;LCDM17 |= 0x03;break;
         case 5:LCDM19 |= 0x01;LCDM18 |= 0x07;LCDM17 |= 0x02;break;
         case 6:LCDM19 |= 0x03;LCDM18 |= 0x07;LCDM17 |= 0x02;break;
         case 7:LCDM19 |= 0x00;LCDM18 |= 0x01;LCDM17 |= 0x03;break;
         case 8:LCDM19 |= 0x03;LCDM18 |= 0x07;LCDM17 |= 0x03;break;
         case 9:LCDM19 |= 0x01;LCDM18 |= 0x07;LCDM17 |= 0x03;break;
        }

     LCDM16 &= 0x18;LCDM15 &= 0x1C;LCDM14 &= 0x1C;    //clear out bits that are related to digi12

    switch(lsb0)
       {
        case 0:LCDM16 |= 0x03;LCDM15 |= 0x05;LCDM14 |= 0x03;break;
        case 1:LCDM16 |= 0x00;LCDM15 |= 0x00;LCDM14 |= 0x03;break;
        case 2:LCDM16 |= 0x02;LCDM15 |= 0x07;LCDM14 |= 0x01;break;
        case 3:LCDM16 |= 0x00;LCDM15 |= 0x07;LCDM14 |= 0x03;break;
        case 4:LCDM16 |= 0x01;LCDM15 |= 0x02;LCDM14 |= 0x03;break;
        case 5:LCDM16 |= 0x01;LCDM15 |= 0x07;LCDM14 |= 0x02;break;
        case 6:LCDM16 |= 0x03;LCDM15 |= 0x07;LCDM14 |= 0x02;break;
        case 7:LCDM16 |= 0x00;LCDM15 |= 0x01;LCDM14 |= 0x03;break;
        case 8:LCDM16 |= 0x03;LCDM15 |= 0x07;LCDM14 |= 0x03;break;
        case 9:LCDM16 |= 0x01;LCDM15 |= 0x07;LCDM14 |= 0x03;break;
       }
         }

  void LcdDisplayTopRow(unsigned int incoming)
        {
         unsigned int temp;
         unsigned int lsb3;
         unsigned int lsb2;
         unsigned int lsb1;
         unsigned int lsb0;

         temp = Dec2BCD(incoming);
         lsb3 = (temp & 0xF000) >>12;
         lsb2 = (temp & 0xF00) >>8;
         lsb1 = (temp & 0xF0) >>4;
         lsb0 = (temp & 0x0F) >>0;

         LcdClearTopRow();

         switch(lsb3)
             {
             case 0:LCDM6W |= 0x1E14;break;
             case 1:LCDM6W |= 0x0C00;break;
             case 2:LCDM6W |= 0x1A0C;break;
             case 3:LCDM6W |= 0x1E08;break;
             case 4:LCDM6W |= 0x0C18;break;
             case 5:LCDM6W |= 0x1618;break;
             case 6:LCDM6W |= 0x161C;break;
             case 7:LCDM6W |= 0x1C00;break;
             case 8:LCDM6W |= 0x1E1C;break;
             case 9:LCDM6W |= 0x1E18;break;
             }

         switch(lsb2)   // digit 2 - top row second from left
            {
            case 0:LCDM8W |= 0x1C16;break;
            case 1:LCDM8W |= 0x0C00;break;
            case 2:LCDM8W |= 0x180E;break;
            case 3:LCDM8W |= 0x1C0A;break;
            case 4:LCDM8W |= 0x0C18;break;
            case 5:LCDM8W |= 0x141A;break;
            case 6:LCDM8W |= 0x141E;break;
            case 7:LCDM8W |= 0x1C00;break;
            case 8:LCDM8W |= 0x1C1E;break;
            case 9:LCDM8W |= 0x1C1A;break;
            }

         switch(lsb1)   // digit 3 - top row third from left
            {
            case 0:LCDM8W |= 0x0200;LCDM10W |= 0x141A;break;
            case 1:LCDM8W |= 0x0000;LCDM10W |= 0x1400;break;
            case 2:LCDM8W |= 0x0200;LCDM10W |= 0x1016;break;
            case 3:LCDM8W |= 0x0000;LCDM10W |= 0x1416;break;
            case 4:LCDM8W |= 0x0000;LCDM10W |= 0x140C;break;
            case 5:LCDM8W |= 0x0000;LCDM10W |= 0x041E;break;
            case 6:LCDM8W |= 0x0200;LCDM10W |= 0x041E;break;
            case 7:LCDM8W |= 0x0000;LCDM10W |= 0x1410;break;
            case 8:LCDM8W |= 0x0200;LCDM10W |= 0x141E;break;
            case 9:LCDM8W |= 0x0000;LCDM10W |= 0x141E;break;
            }

       switch(lsb0)   // digit 4 - lsb
          {
          case 0:LCDM10W |= 0x0B00;LCDM12W |= 0x001A;break;
          case 1:LCDM10W |= 0x0000;LCDM12W |= 0x000A;break;
          case 2:LCDM10W |= 0x0300;LCDM12W |= 0x001C;break;
          case 3:LCDM10W |= 0x0100;LCDM12W |= 0x001E;break;
          case 4:LCDM10W |= 0x0800;LCDM12W |= 0x000E;break;
          case 5:LCDM10W |= 0x0900;LCDM12W |= 0x0016;break;
          case 6:LCDM10W |= 0x0B00;LCDM12W |= 0x0016;break;
          case 7:LCDM10W |= 0x0000;LCDM12W |= 0x001A;break;
          case 8:LCDM10W |= 0x0B00;LCDM12W |= 0x001E;break;
          case 9:LCDM10W |= 0x0900;LCDM12W |= 0x001E;break;
          }
      }


  void LcdDisplayTopRow1(unsigned int incoming)
        {

         LcdClearTopRow();
         unsigned int temp;
         unsigned int lsb[4];
         unsigned int loop_digit = 0;
         unsigned int loop_segment = 0;
         unsigned int roller;

         long int i;

         temp = Dec2BCD(incoming);
         lsb[0] = (temp & 0xF000) >>12;
         lsb[1] = (temp & 0xF00) >>8;
         lsb[2] = (temp & 0xF0) >>4;
         lsb[3] = (temp & 0x0F) >>0;


for(loop_digit = 0; loop_digit <4; loop_digit++)
    {
    roller = segment_data[lsb[loop_digit]];
    loop_segment = 6;

     do{
          if(roller & 0x01) HWREG8(LCD_E_BASE + OFS_LCDM0W + lcd_data[loop_digit][loop_segment*2]) |= lcd_data[loop_digit][loop_segment*2+1];
          roller = roller >> 1;
         }while(loop_segment--);
    }
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

unsigned int Dec2BCD(unsigned int value)
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

/*extern char* itoa(int value, char* result)
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
*/


