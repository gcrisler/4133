/*
 * lcd_display.h
 *
 *  Created on: Aug 6, 2019
 *      Author: gcrisler
 */

#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H


void    LcdInit(void);

unsigned int Dec2BCD(unsigned int);
unsigned long int Dec2BcdLong(unsigned long int);
void LcdDisplayTopRow(unsigned int);
void LcdDisplayMiddleRow(unsigned long int);
void LcdDisplayBottomRow(unsigned long int);
void LcdDisplayTopRow1(unsigned int incoming);
void LcdClearTopRow(void);


#endif /* LCD_DISPLAY_H_ */
