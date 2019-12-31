/*
 * setup.h
 *
 *  Created on: Dec 18, 2019
 *      Author: gcrisler
 */

#ifndef SETUP_H_
#define SETUP_H_

#include "driverlib.h"

void init_gpio(void);
void init_rtc(void);
void init_wdt(void);
void init_lcd(void);
void init_timerA(void);
void init_clock(void);



#endif /* SETUP_H_ */
