#ifndef __ADC_H
#define __ADC_H

#include "avr/io.h"
#include "avr/interrupt.h"
#include "globals.h"

void init_adc(void);
void timer_init(void);

volatile int ADC_POLL_FLAG;

#endif
