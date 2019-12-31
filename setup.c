/*
 * setup.c
 *
 *  Created on: Dec 18, 2019
 *      Author: gcrisler
 */
#include <driverlib.h>

void init_clock(void)
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
    }

void init_gpio(void)
    {
        //Enable S1 internal resistance as pull-Up resistance
        GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
        GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);
        GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN3);
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN7);

        //S1 interrupt enabled
        GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN0);
        GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN1);
        GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN2);
        GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN3);

        //S1 Hi/Lo edge
        GPIO_selectInterruptEdge(GPIO_PORT_P1,GPIO_PIN0,GPIO_HIGH_TO_LOW_TRANSITION);
        GPIO_selectInterruptEdge(GPIO_PORT_P1,GPIO_PIN1,GPIO_HIGH_TO_LOW_TRANSITION);
        GPIO_selectInterruptEdge(GPIO_PORT_P1,GPIO_PIN2,GPIO_HIGH_TO_LOW_TRANSITION);
        GPIO_selectInterruptEdge(GPIO_PORT_P1,GPIO_PIN3,GPIO_HIGH_TO_LOW_TRANSITION);

        //S1 IFG cleared
        GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN0);
        GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN1);
        GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN2);
        GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN3);
        GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN4);
        P1IFG = 0x00;
    }

 void init_rtc(void)
        {
            //Initialize RTC
           RTC_init(RTC_BASE,32768,RTC_CLOCKPREDIVIDER_1);

           RTC_clearInterrupt(RTC_BASE,RTC_OVERFLOW_INTERRUPT_FLAG);

           //Enable interrupt for RTC overflow
           RTC_enableInterrupt(RTC_BASE,RTC_OVERFLOW_INTERRUPT);

           //Start RTC Clock with clock source SMCLK
           RTC_start(RTC_BASE, RTC_CLOCKSOURCE_XT1CLK);
        }

 void init_wdt(void)
        {
         //Initialize WDT module in timer interval mode,
          //with ACLK as source at an interval of 15 ms.
          WDT_A_initIntervalTimer(WDT_A_BASE,WDT_A_CLOCKSOURCE_ACLK,WDT_A_CLOCKDIVIDER_512);

          //Enable Watchdog Interupt
          SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
          SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
        }

 void init_lcd(void)
         {
         LCD_E_setPinAsLCDFunctionEx(LCD_E_BASE, LCD_E_SEGMENT_LINE_0, LCD_E_SEGMENT_LINE_38);

        LCD_E_initParam initParams = {0};
        initParams.clockSource = LCD_E_CLOCKSOURCE_XTCLK;
        initParams.clockDivider = LCD_E_CLOCKDIVIDER_10;
        initParams.muxRate = LCD_E_5_MUX;
        initParams.waveforms = LCD_E_LOW_POWER_WAVEFORMS;
        initParams.segments = LCD_E_SEGMENTS_ENABLED;

        // Init LCD as 5-mux mode
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

         }

 void init_timerA(void)
         {
             //Start timer in continous mode sourced by XTL
            Timer_A_initContinuousModeParam initContParam = {0};
                initContParam.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
                initContParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
                initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
                initContParam.timerClear = TIMER_A_DO_CLEAR;
                initContParam.startTimer = true;
            Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);

            //Initiaze capture mode
            Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_1);

            Timer_A_initCaptureModeParam initCaptureParam = {0};
            initCaptureParam.captureInputSelect = TIMER_A_CAPTURE_INPUTSELECT_Vcc;
            initCaptureParam.captureInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
            initCaptureParam.captureMode = TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE;
            initCaptureParam.captureOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
            initCaptureParam.captureRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
            initCaptureParam.synchronizeCaptureSource = TIMER_A_CAPTURE_SYNCHRONOUS;
            Timer_A_initCaptureMode(TIMER_A1_BASE, &initCaptureParam);

            Timer_A_startCounter( TIMER_A1_BASE,TIMER_A_CONTINUOUS_MODE);
         }
