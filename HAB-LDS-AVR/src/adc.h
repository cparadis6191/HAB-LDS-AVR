#ifndef __ADC_H_
#define __ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stddef.h>

#include "globals.h"


// ADC initialization functions
void adc_init(void);

// Initialize channels 1 and 2 to be used
void adc_channel_init(void);

// Configure channels 1 and 2 to be low-level interrupts
void adc_interrupt_init(void);

// Configure the timer to interrupt every t seconds (t should be less than 256)
void adc_timer_init(int t);

// Start a conversion on the specified pin
void adc_start(int channel_0_pin, int channel_1_pin);

// Used to retrieve factory calibration byte for the ADC
uint8_t adc_read_calibration_byte(uint8_t address);


// Global variables and flags redeclared to be used in ADC code
volatile int g_ADC_RECORD_FLAG;

// Channel results
volatile int g_ADC_CH0_RESULT;
volatile int g_ADC_CH1_RESULT;
// Flag to signal when a conversion is complete on a channel
volatile int g_ADC_CH0_COMPLETE;
volatile int g_ADC_CH1_COMPLETE;

#endif