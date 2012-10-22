#ifndef __ADC_H_
#define __ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "globals.h"

void adc_init(void);
void adc_channel_init(void);
void adc_interrupt_init(void);
void adc_timer_init(void);
void adc_start(uint8_t pin);

uint8_t adc_read_calibration_byte(uint8_t location);

volatile int g_ADC_POLL_FLAG;
volatile uint8_t g_ADC_INDEX;
volatile int g_ADC_RESULT[10];

#endif
