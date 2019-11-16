/*
 * lcd_display.h
 *
 *  Created on: Aug 6, 2019
 *      Author: gcrisler
 */

#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H


void    LcdInit(void);

unsigned int Dec2BCD(int);
unsigned long int Dec2BcdLong(unsigned long int);
void LcdDisplayTopRow(int);
void LcdDisplayMiddleRow(long int);
void LcdDisplayBottomRow(long int);


#endif /* LCD_DISPLAY_H_ */
