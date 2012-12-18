#ifndef __STATE_MACHINE_H_
#define __STATE_MACHINE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "lcd.h"
#include "adc.h"
#include "pc_interface.h"
#include "storage.h"

#define JUMPER_ON (PORTC.IN & PIN0_bm)

// Enumerated states
enum {	ST_NULL = 0,
		ST_INIT,
		ST_IDLE,

		// A/D States
		ST_POLLING_INIT,
		ST_POLLING,
		ST_POLLING_DONE,

		// Custom PC software states
		ST_PC_INIT_COMM,
		ST_PC_CONNECTED,
		ST_PC_DISCONNECT };


// Call this to enter the state machine
void state_machine(void);

// Initialize the internal oscillator (to 32MHz)
void clock_32MHz_init(void);

// Initialize the real-time clock
void clock_32kHz_init(void);

// Initialize DFLL
void clock_DFLL_init(void);

// Initialize interrupts
void main_interrupts_init(void);

// Initialize the interrupts for the jumper and 3.3V line on the FT232 chip
void input_init(void);

// Resolve the angle from the 10 sensor results
double resolve_angle(int *sensor_results);

#endif