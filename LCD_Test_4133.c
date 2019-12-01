
//*****************************************************************************
//! LCD_E, 4-Mux example, display a string "123456" on LCD.
//! This examples shows how to config LCD_E in 4-Mux mode with DriverLib
//! functions on the MSP430FR4133 LaunchPad. It uses the Mode 3 (see User's Guide
//! for details) for voltage generation. Therefore the charge pump and bias 
//! voltage generator are used to generate V1, V2 and V4. V5 is connected to 
//! ground. Three external 0.1 uF capacitors are required to connected to R13, 
//! R23 and R33 respectively, and one 4.7 uF capacitor needs to connect between 
//! charge pump LCDCAP0 and LCDCAP1.
//!
//! It uses LCD_E L1~L4 as COMs and maps pin to the corresponding COM (e.g.
//! L1 -> COM1), and L4~L39 (except L27~L35) as segment pins.
//! f(LCD) = 32768Hz/((7+1)*16) = 256Hz, ACLK = default REFO ~32768Hz,
//! MCLK = SMCLK = default DCODIV ~1MHz.
//!
//!              MSP-EXP430FR4133
//!                 LaunchPad
//!             -----------------
//!         /|\|                 |
//!          | |              XIN|--
//! GND      --|RST              |  ~32768Hz
//!  |         |             XOUT|--
//!  |--0.1uF--|R13              |
//!  |--0.1uF--|R23      (L3)COM3|----------------|
//!  |--0.1uF--|R33      (L2)COM2|---------------||
//!          --|LCDC2    (L1)COM1|--------------|||
//!     4.7uF  |         (L0)COM0|-------------||||
//!          --|LCDC1            |    -------------
//!            |           L4~L39|---| 1 2 3 4 5 6 |
//!            |   except L27~L35|    -------------
//!            |                 |       TI LCD
//!
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - LCD_E
//! - GPIO Port peripheral (for LCD_E pins)
//!
//!
//*****************************************************************************
#include "driverlib.h"
#include "lcd_display.h"


void main(void)
{
	// Hold watchdog
    WDT_A_hold(WDT_A_BASE);

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

  //  P7OUT = 0xFF;

    // L0~L26 & L36~L39 pins selected
    LCD_E_setPinAsLCDFunctionEx(LCD_E_BASE, LCD_E_SEGMENT_LINE_0, LCD_E_SEGMENT_LINE_38);

    LCD_E_initParam initParams = {0};
    initParams.clockSource = LCD_E_CLOCKSOURCE_XTCLK;
    initParams.clockDivider = LCD_E_CLOCKDIVIDER_10;
    initParams.muxRate = LCD_E_5_MUX;
    initParams.waveforms = LCD_E_LOW_POWER_WAVEFORMS;
    initParams.segments = LCD_E_SEGMENTS_ENABLED;

    // Init LCD as 8-mux mode
    LCD_E_init(LCD_E_BASE, &initParams);

    // LCD Operation - Mode 3, internal 3.08v, charge pump 256Hz
    LCD_E_setVLCDSource(LCD_E_BASE, LCD_E_INTERNAL_REFERENCE_VOLTAGE, LCD_E_EXTERNAL_SUPPLY_VOLTAGE);
    LCD_E_setVLCDVoltage(LCD_E_BASE, LCD_E_REFERENCE_VOLTAGE_2_72V);

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
    LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_4, LCD_E_MEMORY_COM4);

  //  LCD_E_on(LCD_E_BASE);

    LCDM32W = 0x00;
    LCDM34W = 0x00;
    LCDM36W = 0x00;

    LCD_E_on(LCD_E_BASE);

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
    LcdDisplayMiddleRow(99999999);
     LcdDisplayBottomRow(99999999);
     //LcdDisplayTopRow(8300);
    // Turn on LCD
   LCD_E_on(LCD_E_BASE);

    // Enter LPM3.5
    PMM_turnOffRegulator();
    __bis_SR_register(LPM4_bits | GIE);
}

