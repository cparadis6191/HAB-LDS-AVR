#ifndef __STATE_MACHINE_H_
#define __STATE_MACHINE_H_


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

#endif