#ifndef __PC_INTERFACE_H_
#define __PC_INTERFACE_H_

#include <stdio.h>
#include <avr/io.h>

#define PC_POWER PORTC.IN & PIN0_bm

// PC starts by sending an initialization byte
#define PC_INIT 0xAB
// AVR sends back an acknowledgment byte
#define AVR_ACK 0XBB

// PC sends a request for data
#define PC_DATA_REQUEST 0xCB
// Header signifying the end of sent data
#define AVR_DATA_END 0xDB
// PC sends a header indicating that the next byte sent contains the polling interval
#define PC_POLLING_INTERVAL 0xEB

// PC sends this to reset the EEPROM data pointer back to the start of data memory
#define PC_ERASE_DATA 0xAC
// PC sends this when it wants to close communication or when the AVR times out
#define PC_COMM_CLOSE 0xFB

// Use USARTD1 for communication, PD6 is Rx, PD7 is Tx
void USARTD1_init(void);
int USARTD1_putchar(char c, FILE *stream);
int USARTD1_getchar(FILE *stream);

// Initialize communication to the PC
void pc_interface_init(void);

#endif