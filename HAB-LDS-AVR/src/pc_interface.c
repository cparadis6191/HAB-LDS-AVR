#include "pc_interface.h"

// Use the UART lines in PORTC. PORTC[2:3]
void USARTC1_init(void) {
	// Set TxD as output
	PORTE.DIRSET |= PIN7_bm;
	// Set the TxD pin value high
	PORTE.OUTSET |= PIN7_bm;
	
	
	// Values taken from http://www.avrcalc.elektronik-projekt.de/xmega/baud_rate_calculator
	// Values for 38400 baud
	int bsel = 3269;
	uint8_t bscale = -6;

	// Set the baud rate and frame format
	USARTC1.BAUDCTRLA = (uint8_t) bsel;
	USARTC1.BAUDCTRLB = (bscale << 4) | (bsel >> 8);
	
	// Frame for the Grove Serial LCD is 8n1
	// Set character size to 8-bits (011)

	USARTC1.CTRLC |= USART_CHSIZE_8BIT_gc;
	// Parity initialized to no parity (00)
	// Stop bits set to zero by default (0)
	
	// Enable the transmit and receive lines
	USARTC1.CTRLB |= (USART_TXEN_bm | USART_RXEN_bm);

	return;
}

void pc_interface_init(void) {
	
	return;
}

int USARTC1_putchar(char c) {
	// Wait until the transmit register is empty
	while (!(USARTC1.STATUS & USART_DREIF_bm));
	
	// Write the data
	USARTC1.DATA = c;
	
	return 0;
}

int USARTC1_getchar(void) {
	// Wait until the Receive complete interrupt flag is set
	while (!(USARTC1.STATUS & USART_RXCIF_bm));
	
	// Read the data
	int c = USARTC1.DATA;
	
	return c;
}