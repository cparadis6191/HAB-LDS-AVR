#ifndef __LCD_USART_H_
#define __LCD_USART_H_

#include <stdio.h>
#include <avr/io.h>

#include "lcd_library.h"

void USARTC1_init(void);
int USARTC1_putchar(char c, FILE *stream);
int USARTC1_getchar(FILE *stream);

// Initialize the LCD
void lcd_init(void);

// Clear the display and put the cursor in the upper right
void lcd_clear_display(void);
void lcd_backlight_off(void);
void lcd_backlight_on(void);

#endif