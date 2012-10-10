#include "lcd.h"

// Use the UART lines in PORTC. PORTC[2:3]
void USARTC0_init( void ) {
	// Set the TxD pin value high
	PORTC.DATA |= PIN3_bm;
	// Set TxD as output
	PORTC.DIR |= PIN3_bm;
	
	// Set the baud rate and frame format
	// Lower eight bits of BSEL are in BAUDCTRLA
	USARTC0.BAUDCTRLA = (uint8_t) 3317;
	
	// Upper four bits of BSEL are the lower four of BAUDCTRLB
	// Upper four bits of BAUDCTRLB are BSCALE
	USARTC0.BAUDCTRLB = ( (-4 << 4) | (3317 >> 8) );
	
	// Default Arduino USART frame is 8n1
	// Set the mode of operation
	// Set character size to 8-bits (011)
	USARTC0.CTRLC |= USART_CHSIZE_8BIT_gc;
	
	// Parity initialized to no parity (00)
	// Stop bits set to zero by default (0)
	
	// Enable the transmit and receive lines
	PORTC.DIR |= USART_TXEN_bm | USART_RXEN_bm;
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