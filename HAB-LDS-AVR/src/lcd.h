#ifndef __LCD_USART_H_
#define __LCD_USART_H_

#include "globals.h"
#include "lcd_library.h"

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define LCD_POWER PORTC.IN & PIN2_bm

// Use USARTD0 for communication, PD2 is Rx, PD3 is Tx
void USARTD0_init(void);
int USARTD0_putchar(char c, FILE *stream);
int USARTD0_getchar(FILE *stream);

// Initialize the LCD
void lcd_init(void);

// Clear the display and put the cursor in the upper right
void lcd_clear_display(void);
void lcd_return_home(void);
void lcd_backlight_off(void);
void lcd_backlight_on(void);
void lcd_cursor_set(int x, int y);

#endif