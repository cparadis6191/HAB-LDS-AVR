#ifndef __STORAGE_H_
#define __STORAGE_H_

#include <avr/eeprom.h>


// Store the address of the last data point written in EEPROM 0x0000
// This is two bytes wide so takes up 0x0000-0x0001
#define EEPROM_LAST_DATA EEPROM_START

// A constant place to store the polling interval in EEPROM 0x0002
#define EEPROM_POLLING_INTERVAL (EEPROM_START + 2)

// EEPROM 0x0003 is not used currently
// Start data at EEPROM 0x0004
#define EEPROM_DATA_START (EEPROM_START + 4)

void storage_write_angle(double resolved_angle, int* memory_location);

#endif
