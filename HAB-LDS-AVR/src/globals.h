#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#define F_CPU 32000000UL

// Global flag indicating when to start an ADC conversion
// Will be controlled by a timer
volatile extern int ADC_POLL_FLAG;

volatile extern int ADC_RESULT[10];

#endif