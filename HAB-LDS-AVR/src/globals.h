#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#define F_CPU 32000000UL

// Declare global flags indicating when to start an ADC conversion
// Will be controlled by a timer
extern volatile int g_ADC_RECORD_FLAG;
extern volatile int g_ADC_CH0_COMPLETE;
extern volatile int g_ADC_CH1_COMPLETE;

// Global buffer to store results, and an index to keep track of which LED we're on
extern volatile int g_ADC_CH0_RESULT;
extern volatile int g_ADC_CH1_RESULT;

#endif