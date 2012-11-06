#ifndef __GLOBALS_H_
#define __GLOBALS_H_

// Declare global flags indicating when to start an ADC conversion
// Will be controlled by a timer
extern volatile int g_ADC_RECORD_FLAG;
extern volatile int g_ADC_CONVERSION_COMPLETE_CHANNEL_0;
extern volatile int g_ADC_CONVERSION_COMPLETE_CHANNEL_1;
extern volatile int g_ADC_RECORD_DELAY;

// Global buffer to store results, and an index to keep track of which LED we're on
extern volatile int g_ADC_RESULT_CHANNEL_0;
extern volatile int g_ADC_RESULT_CHANNEL_1;

#endif