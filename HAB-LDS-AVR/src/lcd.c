#include "lcd.h"
#define F_CPU 32000000

// Use the UART lines in PORTC. PORTC[2:3]
void USARTC0_init( void ) {
	// Set TxD as output
	PORTC.DIRSET |= PIN3_bm;
	// Set the TxD pin value high
	PORTC.OUTSET |= PIN3_bm;
	
	// Upper four bits of BSEL are the lower four of BAUDCTRLB
	// Upper four bits of BAUDCTRLB are BSCALE
	int bsel = 3421;
	uint8_t bscale = -4;

	// Set the baud rate and frame format
	// Lower eight bits of BSEL are in BAUDCTRLA
	USARTC0.BAUDCTRLA = (uint8_t) bsel;
	USARTC0.BAUDCTRLB = (bscale << 4) | (bsel >> 8);
	
	USARTC0.CTRLA = 0x00;
	USARTC0.CTRLB = 0x00;
	USARTC0.CTRLC = 0x00;
	
	// Default Arduino USART frame is 8n1
	// Set the mode of operation
	// Set character size to 8-bits (011)
	//USARTC0.CTRLC |= USART_CHSIZE_8BIT_gc;
	//USARTC0.CTRLC = (USARTC0.CTRLC & ~USART_CHSIZE_gm) |  USART_CHSIZE_8BIT_gc;
	USARTC0.CTRLC = 0b00000011;
	// Parity initialized to no parity (00)
	// Stop bits set to zero by default (0)
	
	// Enable the transmit and receive lines
	USARTC0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
}

void lcd_init( void ) {
	
}

int USARTC0_putchar( char c ) {
	// Wait until the transmit register is empty
	while ( !(USARTC0.STATUS & USART_DREIF_bm) );
	
	// Write the data
	USARTC0.DATA = c;
	
	return 0;
}

int USARTC0_getchar( void ) {
	
	// Wait until the Receive complete interrupt flag is set
	while ( !(USARTC0.STATUS & USART_RXCIF_bm) );
	
	// Read the data
	int c = USARTC0.DATA;
	
	return c;
}