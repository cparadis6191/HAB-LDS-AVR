#ifndef __PC_INTERFACE_H_
#define __PC_INTERFACE_H_

#include <stdio.h>
#include <avr/io.h>

// PC starts by sending an initialization byte
#define PC_INIT 0xAB
// AVR sends back an acknowledgment byte
#define AVR_ACK 0XBB
// PC sends a request for data
#define PC_DATA_REQUEST_HEADER 0xCB
// Header signifying the end of sent data
#define AVR_DATA_END_HEADER 0xDB
// PC sends a header indicating that the next byte sent contains the polling interval
#define PC_POLLING_INTERVAL_HEADER 0xEB
// PC sends this when it wants to close communication or when the AVR times out
#define PC_COMM_CLOSE 0xFB


void USARTC0_init(void);
int USARTC0_putchar(char c, FILE *stream);
int USARTC0_getchar(FILE *stream);

// Initialize communication to the PC
void pc_init(void);

#endif