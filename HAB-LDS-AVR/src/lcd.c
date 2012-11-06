#include "lcd.h"

// Use the UART lines in PORTD. PORTD[2:3]
void USARTD0_init(void) {
	// Set TxD as output
	PORTD.DIRSET |= PIN3_bm;
	// Set the TxD pin value high
	PORTD.OUTSET |= PIN3_bm;
	
	// Values taken from http://www.avrcalc.elektronik-projekt.de/xmega/baud_rate_calculator
	// Values for 9600 baud
	int bsel = 3462;
	// Manually raising the baudrate cause the frequency is probably drifting
	bsel = 3717;
	uint8_t bscale = -4;

	// Set the baud rate and frame format
	USARTD0.BAUDCTRLA = (uint8_t) bsel;
	USARTD0.BAUDCTRLB = (bscale << 4) | (bsel >> 8);
	
	// Frame for the Grove Serial LCD is 8n1
	// Set character size to 8-bits (011)

	USARTD0.CTRLC = USART_CHSIZE_8BIT_gc;
	// Parity initialized to no parity (00)
	// Stop bits set to zero by default (0)
	
	// Enable the transmit and receive lines
	USARTD0.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);

	return;
}

void lcd_init(void) {
	// Put the LCD into a known state
	USARTD0_putchar(SLCD_CONTROL_HEADER, NULL);
	USARTD0_putchar(SLCD_POWER_OFF, NULL);

	// Blocking call, need to fix
	while (USARTD0_getchar(NULL) != UART_READY);// if (TIMEOUT) break;

	USARTD0_putchar(SLCD_CONTROL_HEADER, NULL);
	USARTD0_putchar(SLCD_POWER_ON, NULL);
	USARTD0_putchar(SLCD_INIT_ACK, NULL);

	// Another blocking call, could cause issues
	// Wait
	while (USARTD0_getchar(NULL) != SLCD_INIT_DONE);// if (TIMEOUT) break;
	
	// Put the LCD in character mode
	USARTD0_putchar(SLCD_CHAR_HEADER, NULL);

	return;
}

// Clear the display and put the cursor in the upper left
void lcd_clear_display(void) {
	USARTD0_putchar(SLCD_CONTROL_HEADER, NULL);
	USARTD0_putchar(SLCD_CLEAR_DISPLAY, NULL);

	// Put the LCD back in character mode
	USARTD0_putchar(SLCD_CHAR_HEADER, NULL);

	return;
}

// Really don't want to use this, it kills battery
void lcd_backlight_on(void) {
	USARTD0_putchar(SLCD_CONTROL_HEADER, NULL);
	USARTD0_putchar(SLCD_BACKLIGHT_ON, NULL);

	// Put the LCD back in character mode
	USARTD0_putchar(SLCD_CHAR_HEADER, NULL);

	return;
}

void lcd_backlight_off(void) {
	// Turn on the backlight
	USARTD0_putchar(SLCD_CONTROL_HEADER, NULL);
	USARTD0_putchar(SLCD_BACKLIGHT_OFF, NULL);

	// Put the LCD back in character mode
	USARTD0_putchar(SLCD_CHAR_HEADER, NULL);

	return;
}

int USARTD0_putchar(char c, FILE *stream) {
	// Wait until the transmit register is empty
	while (!(USARTD0.STATUS & USART_DREIF_bm));
	
	// Write the data
	USARTD0.DATA = c;
	
	return 0;
}

int USARTD0_getchar(FILE *stream) {
	// Wait until the Receive complete interrupt flag is set
	while (!(USARTD0.STATUS & USART_RXCIF_bm));
	
	// Read the data
	return USARTD0.DATA;
}