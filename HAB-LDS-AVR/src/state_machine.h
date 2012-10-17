#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "avr/interrupt.h"
#include "stdio.h"
#include "stdlib.h"

#include "globals.h"
#include "lcd.h"
#include "adc.h"
#include "pc_interface.h"
#include "sd.h"

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

volatile extern int ST_STATE;

// Call this to enter the state machine
void state_machine(void);

// Initialize the internal oscillator (to 32MHz)
void clock_32MHz_init(void);

// Initialize the real-time clock
void clock_32kHz_init(void);

// Initialize interrupts
void interrupts_init(void);

#endif