#ifndef __PC_INTERFACE_H_
#define __PC_INTERFACE_H_

#include <stdio.h>
#include <avr/io.h>

#include "lcd_library.h"

void USARTC0_init(void);
int USARTC0_putchar(char c, FILE *stream);
int USARTC0_getchar(FILE *stream);

// Initialize communication to the PC
void pc_init(void);

#endif