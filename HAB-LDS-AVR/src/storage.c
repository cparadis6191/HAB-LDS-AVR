#include "storage.h"


void storage_write_angle(double resolved_angle, int* memory_location) {
	if (*memory_location <= (EEPROM_END - 3)) {
		// Write the data to EEPROM
		// Multiply by ten and store the result as an int so we can use only two bytes of storage
		eeprom_write_word((void *) *memory_location, (int) (resolved_angle*10));

		// Increment to point to the next data point
		*memory_location += 2;

		// Write to EEPROM and store the address of the latest angle
		eeprom_write_word((void *) EEPROM_LAST_DATA, *memory_location);
	}


	return;
}