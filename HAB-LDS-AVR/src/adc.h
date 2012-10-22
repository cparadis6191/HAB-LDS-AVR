#ifndef __ADC_H_
#define __ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stddef.h>

#include "globals.h"

// ADC initialization functions
void adc_init(void);
void adc_channel_init(void);
void adc_interrupt_init(void);
void adc_timer_init(void);

// Start a conversion on the specified pin
void adc_start(uint8_t pin);

uint8_t adc_read_calibration_byte(uint8_t location);

// Global variables and flags redeclared to be used in adc code
volatile int g_ADC_POLL_FLAG;
volatile uint8_t g_ADC_INDEX;
volatile int g_ADC_RESULT[10];

#endif
