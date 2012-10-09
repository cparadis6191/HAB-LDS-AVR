#ifndef __LCD_H_
#define __LCD_H_

#include "stdio.h"
#include "avr/io.h"

void init_UART( void );
static int uart_putchar( char c, FILE *stream );

#endif
