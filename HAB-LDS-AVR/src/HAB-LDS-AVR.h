#ifndef __DIRECTION_SENSOR_H_
#define __DIRECTION_SENSOR_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "state_machine.h"
#include "lcd.h"
#include "adc.h"
#include "pc_interface.h"
#include "storage.h"


#define JUMPER_ON (PORTC.IN & PIN0_bm)

// Initialize the internal oscillator (to 32MHz)
void clock_32MHz_init(void);

// Initialize the real-time clock
void clock_32kHz_init(void);

// Initialize DFLL
void clock_DFLL_init(void);

// Initialize interrupts
void main_interrupts_init(void);

// Initialize the interrupts for the jumper and 3.3V line on the FT232 chip
// Initialize the LED
void input_io(void);

// Resolve the angle from the 10 sensor results
double resolve_angle(int *sensor_results);

#endif