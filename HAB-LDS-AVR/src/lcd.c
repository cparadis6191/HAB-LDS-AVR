#include "lcd.h"

// Use the UART lines in PORTC. PORTC[2:3]

void init_UART( void ) {
	// Set the TxD pin value high
	
	// Set the TxD as output

	// Set the baud rate and frame format
	USARTC0.BAUDCTRLA |= ( 1 << 1 );
	USARTC0.BAUDCTRLB |= ( 1 << 1 );

	// Set the mode of operation

	// Set character size to 8-bits
	USARTC0.CTRLC |= ( 1 << USART_CHSIZE0_bp );
	USARTC0.CTRLC |= ( 1 << USART_CHSIZE1_bp );
	USARTC0.CTRLC &= ~( 1 << USART_CHSIZE2_bp );

	// Enable the transmitter or the receiver

}

// USART Put character used so printf can be used
// Examples and ideas taken from http://blog.frankvh.com/2009/11/14/atmel-xmega-printf-howto/
int uart_putchar( char c, FILE *stream ) {

}
