#ifndef __DIRECTION_SENSOR_H_
#define __DIRECTION_SENSOR_H_

#include "avr/io.h"
#include "avr/interrupt.h"
#include "globals.h"
#include "state_machine.h"
#include "lcd.h"
#include "adc.h"
#include "bluetooth.h"
#include "sd.h"

// Initialize the internal oscillator (to 32MHz)
void clock_32MHz_init(void);
void clock_32kHz_init(void);

void interrupts_init(void);

#endif