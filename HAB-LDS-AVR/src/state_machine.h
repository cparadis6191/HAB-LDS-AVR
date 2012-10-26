#ifndef __STATE_MACHINE_H_
#define __STATE_MACHINE_H_

#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "lcd.h"
#include "adc.h"
#include "pc_interface.h"
#include "storage.h"

// Global variable to hold the current state of the system
volatile extern int ST_STATE;

// Enumerated states
enum {	ST_NULL = -1,
		ST_INIT,
		ST_IDLE,

		// A/D States
		ST_POLLING_INIT,
		ST_POLLING,
		ST_POLLING_DONE,

		// Custom PC software states
		ST_PC_INIT_COMM,
		ST_PC_CONNECTED,
		ST_PC_SEND_DATA,
		ST_PC_RECEIVE_SETTINGS,
		ST_PC_DISCONNECT };

// Call this to enter the state machine
void state_machine(void);

// Initialize the internal oscillator (to 32MHz)
void clock_32MHz_init(void);

// Initialize the real-time clock
void clock_32kHz_init(void);

// Initialize interrupts
void main_interrupts_init(void);

// Resolve the angle from the 10 sensor results
double resolve_angle(void);

#endif