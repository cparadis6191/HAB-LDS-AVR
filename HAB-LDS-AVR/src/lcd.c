#include "lcd.h"

// Use the UART lines in PORTC. PORTC[6:7]
void USARTC1_init(void) {
	// Set TxD as output
	PORTC.DIRSET |= PIN7_bm;
	// Set the TxD pin value high
	PORTC.OUTSET |= PIN7_bm;
	
	// Values taken from http://www.avrcalc.elektronik-projekt.de/xmega/baud_rate_calculator
	// Values for 9600 baud
	int bsel = 3317;
	uint8_t bscale = -4;

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

void lcd_init(void) {
	while (USARTC1_getchar(NULL) != UART_READY);// if (TIMEOUT) break;

	USARTC1_putchar(SLCD_CONTROL_HEADER, NULL);
	USARTC1_putchar(SLCD_POWER_ON, NULL);
	USARTC1_putchar(SLCD_INIT_ACK, NULL);

	// Wait
	while (USARTC1_getchar(NULL) != SLCD_INIT_DONE);// if (TIMEOUT) break;
}

int USARTC1_putchar(char c, FILE *stream) {
	// Wait until the transmit register is empty
	while (!(USARTC1.STATUS & USART_DREIF_bm));
	
	// Write the data
	USARTC1.DATA = c;
	
	return 0;
}

int USARTC1_getchar(FILE *stream) {
	// Wait until the Receive complete interrupt flag is set
	while (!(USARTC1.STATUS & USART_RXCIF_bm));
	
	// Read the data
	return (int) USARTC1.DATA;
}