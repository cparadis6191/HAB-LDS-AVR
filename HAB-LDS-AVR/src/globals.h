#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#define F_CPU 32000000UL

// Global flag indicating when to start an ADC conversion
// Will be controlled by a timer
volatile extern int g_ADC_RECORD_FLAG;
volatile extern int g_ADC_RECORD_DELAY;

// Global buffer to store results, and an index to keep track of which LED we're on
volatile extern uint8_t g_ADC_INDEX;
volatile extern int g_ADC_RESULT[10];

#endif