#include "state_machine.h"

void state_machine(void) {
	volatile int ST_STATE = ST_NULL;
	
	while (1) {
		switch (ST_STATE) {
			case ST_POWER_ON:
				
				continue;

			case ST_INIT:

				continue;

			case ST_POLLING_START:

				continue;

			case ST_POLLING:

				continue;

			case ST_POLLING_DONE:

				continue;

			case ST_BLUETOOTH_QUERY:
			
				continue;

			case ST_BLUETOOTH_CONNECT:
			
				continue;

			case ST_BLUETOOTH_COMMUNICATE:
			
				continue;

			case ST_BLUETOOTH_DISCONNECT:
			
				continue;

			default:

				continue;
		}
	}
}