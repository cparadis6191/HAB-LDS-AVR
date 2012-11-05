#include "pc_interface.h"

// Use the UART lines in PORTC. PORTC[2:3]
void USARTD1_init(void) {
	// Set TxD as output
	PORTD.DIRSET |= PIN7_bm;
	// Set the TxD pin value high
	PORTD.OUTSET |= PIN7_bm;
	
	// Values taken from http://www.avrcalc.elektronik-projekt.de/xmega/baud_rate_calculator
	// Values for 9600 baud
	int bsel = 3317;
	uint8_t bscale = -4;

	// Set the baud rate and frame format
	USARTD1.BAUDCTRLA = (uint8_t) bsel;
	USARTD1.BAUDCTRLB = (bscale << 4) | (bsel >> 8);
	
	// Frame for the PC interface is 8n1
	// Set character size to 8-bits (011)

	USARTD1.CTRLC = USART_CHSIZE_8BIT_gc;
	// Parity initialized to no parity (00)
	// Stop bits set to zero by default (0)
	
	// Enable the transmit and receive lines
	USARTD1.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);

	return;
}

void pc_interface_init(void) {
	
	return;
}

int USARTD1_putchar(char c, FILE *stream) {
	// Wait until the transmit register is empty
	while (!(USARTD1.STATUS & USART_DREIF_bm));
	
	// Write the data
	USARTD1.DATA = c;
	
	return 0;
}

int USARTD1_getchar(FILE *streamvoid) {
	// Wait until the Receive complete interrupt flag is set
	while (!(USARTD1.STATUS & USART_RXCIF_bm));
	
	// Read the data
	return USARTD1.DATA;
}