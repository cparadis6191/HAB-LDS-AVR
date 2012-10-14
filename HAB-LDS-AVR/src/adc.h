#ifndef __ADC_H
#define __ADC_H

#include "avr/io.h"
#include "avr/interrupt.h"
#include "globals.h"

void adc_init(void);
void timer_init(void);

volatile int ADC_POLL_FLAG;
volatile int ADC_INDEX;
volatile int ADC_RESULT[10];

#endif
