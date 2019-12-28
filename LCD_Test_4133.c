//
//!
//*****************************************************************************
#include "driverlib.h"
#include "lcd_display.h"
#include "setup.h"

//#include "Board.h"
#include <stdbool.h>
unsigned int z;
unsigned int i;
//unsigned int j = 0;
//unsigned int loop = 0;
struct buttons_declare
        {
        unsigned int menu;
        unsigned int reset;
        unsigned int size;
        unsigned int paddle;
        unsigned int current_count;
        unsigned int past_count;
        bool    flag_update;
        bool    lock;
        };
struct buttons_declare buttons;


void main(void)
{

    init_clock();
	init_wdt();
	init_gpio();
	init_rtc();
	init_timerA();
	init_lcd();

    PMM_unlockLPM5();

     LcdDisplayTopRow(0000);
    // LcdDisplayTopRow(1111);
   //  LcdDisplayTopRow(2222);
    // LcdDisplayTopRow(3333);
    // LcdDisplayTopRow(4444);
     //LcdDisplayTopRow(5555);
    // LcdDisplayTopRow(6666);
    // LcdDisplayTopRow(7777);
   //  LcdDisplayTopRow(8888);
   //  LcdDisplayTopRow(9999);
   // LcdDisplayMiddleRow(88888888);
   // LcdDisplayBottomRow(88888888);
   // LcdDisplayTopRow(buttons.menu +2);
    // Turn on LCD
 //    LCD_E_setMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_5, 0x1C);
  //   LCD_E_setMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_8, 0x01);
 //    LCD_E_setMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_35, 0x04);
   //  LCD_E_setMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_15, 0x18);
   //  LCD_E_setMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_19, 0x1C);
    // LCD_E_setMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_32, 0x10);

   LCD_E_on(LCD_E_BASE);


    // Enter LPM3.5
 //   PMM_turnOffRegulator();
    __bis_SR_register(LPM4_bits | GIE);

   //For debugger
      __no_operation();
}
//******************************************************************************
//
//This is the PORT1_VECTOR interrupt vector service routine

#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void)
        {
           unsigned long int k;


           switch(_even_in_range(P1IV, 16))
                {
                case  1:{
                    LcdDisplayBottomRow(11);
                        //enter key pressed
                       // k = 0;
                       // while(enter != 0)
                       // if(++k > 500000)
                       //     {
                       //     k=0;
                       //     calibration();
                       //     }
                        }break;
                case  2:    //middle button(plus & reset) depressed
                        {
                            LcdDisplayBottomRow(22);
                         // resettable_total = 0;
                         // display_total();
                        }break;

                case  4:      //right button(minus & size) depressed
                        {
                            LcdDisplayBottomRow(44);
                           //display_schedule(1);
                         // display_size(size_pointer);       //print out size and schedule of pipe
                         // delay(300000);
                         // display_units();   //show units
                         // delay(300000);
                         // display_row_1(adjust,1);          //print out calibration factor
                         // display_adjust();
                         // delay(300000);
                        }break;
                case 6: {
                        LcdDisplayBottomRow(66);

                        }break;

                case 8: {
                        //    TA1CCTL1 |= 0x1000;
                          //  TA1CCTL1 &= 0x7FFF;
                        TA1CCTL1^= 0x1000;   //toggle the voltage to 0 to initiate a interrupt

                        }break;
                }

                P1IFG = 00;
             }

#pragma vector = RTC_VECTOR
     __interrupt

     void RTC_ISR (void)
         {
         switch (__even_in_range(RTCIV,2))
             {
             case 0: break;  //No interrupts
             case 2:         //RTC overflow
                 LcdDisplayTopRow(i++);
                 buttons.flag_update = true;
                 break;
             default: break;
             }
         }


#pragma vector=WDT_VECTOR
__interrupt

void WDT_A_ISR (void)
            {


            if(!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN0))
                    {
                    if(!buttons.lock) buttons.menu++;
                    WDT_A_hold(WDT_A_BASE);
                    buttons.lock = true;
                    }
            else if(!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1))
                    {
                    if(!buttons.lock)buttons.reset++;
                    WDT_A_hold(WDT_A_BASE);
                    buttons.lock = true;
                    }
            else if (!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2))
                    {
                    if(!buttons.lock)buttons.size++;
                    WDT_A_hold(WDT_A_BASE);
                    buttons.lock = true;
                    }

            else if (!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3))
                    {
                    if(!buttons.lock)buttons.paddle++;
                    WDT_A_hold(WDT_A_BASE);
                    buttons.lock = true;
                    }

            else
                    {

                    GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN_ALL8);
                    buttons.lock = false;
                    WDT_A_hold(WDT_A_BASE);
                    }
            //LcdDisplayTopRow(buttons.paddle);

            }

#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A(void)
        {  //LcdDisplayBottomRow(TA1IV);

          switch(_even_in_range(TA1IV, 14))
            {
            case  0:break;
            case  2:
            {
              buttons.past_count = buttons.current_count;
              buttons.current_count = TA1CCR1;
              TA1CTL &= 0xFB0;
              LcdDisplayBottomRow(buttons.current_count - buttons.past_count);
            }

                    break;      //ccr1 not used
            case  4:{           //ccr2 interrupt
                    //arrows ^=1; //toggle between 1 and 0
                    //if(arrows == 0 )
                       // {
                          // light the lower  paddle
                          //clear the upper icon
                       // }
                     // else
                     //  {
                        // light the upper paddle
                        // clear the paddles
                      //  }

                  //  rate_counter = rate_counter + TACCR2;
                  //  count_temp = count_temp + totalizer_adder;
                  //  while(count_temp >= 10000)
                  //      {
                  //      count_temp = count_temp - 10000; //when the register excedes 10000 increment totalizers.
                  //      total++;            //increment total
                  //      resettable_total++;     //increment resetable total
                  //      }
                  //  if (resettable_total > 99999999) resettable_total = 0;
                  //  if (total > 99999999) total = 0;
                  //  ticker++;               //used in figure out the flow rate
                  //  TACTL |= 0x0004;            //reset the Timer A
                  //  if (data_check > 0) data_check--;   //display flow rate and not 0
                    }break;

                case 14:{
                       // LcdDisplayBottomRow(z++);
                        //timer overflow
                        // clear the paddles
                        //print 0.0 if no flow
                        //print 0.0 if no flow

                        }break;
            }
        }
