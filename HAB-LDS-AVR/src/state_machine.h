#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

enum {	ST_NULL = -1,
		ST_POWER_ON,
		ST_INIT,
		ST_POLLING_START,
		ST_POLLING,
		ST_POLLING_DONE,
		ST_BLUETOOTH_QUERY,
		ST_BLUETOOTH_CONNECT,
		ST_BLUETOOTH_COMMUNICATE,
		ST_BLUETOOTH_DISCONNECT };

volatile extern int ST_STATE;

void state_machine(void);

#endif