#ifndef __LCD_USART_H_
#define __LCD_USART_H_

#include <stdio.h>
#include <avr/io.h>

#include "lcd_library.h"

void USARTC0_init(void);
int USARTC0_putchar(char c, FILE *stream);
int USARTC0_getchar(void);

void lcd_init(FILE *stream);

#endif
