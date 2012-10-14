#ifndef __DIRECTION_SENSOR_H_
#define __DIRECTION_SENSOR_H_

#include "avr/io.h"
#include "state_machine.h"
#include "bluetooth.h"
#include "lcd.h"
#include "sd.h"
#include "adc.h"

// Initialize the internal oscillator (to 32MHz)
void clock_init(void);

void interrupts_init(void);

#endif