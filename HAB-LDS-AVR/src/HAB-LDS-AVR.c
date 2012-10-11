#include "HAB-LDS-AVR.h"

static FILE myio = FDEV_SETUP_STREAM(USARTC0_putchar, USARTC0_getchar, _FDEV_SETUP_RW);

int main( void ) {
	USARTC0_init();
	clock_init();
	lcd_init();
	
	stdout = &myio;
	stdin = &myio;
	
	char temp = "";
	while( 1 ) {
		scanf("%c", temp);
		printf("%c", temp);
	}

	return 0;
}

// Set the chip to run on the 32MHz internal oscillator
void clock_init( void ) {
	
	// Allow changing of CLK.CTRL
	CCP = CCP_IOREG_gc;
	// Start 32MHz RC oscillator
	OSC.CTRL = OSC_RC32MEN_bm;
	
	// Wait for ready
	while ( !(OSC.STATUS & OSC_RC32MRDY_bm) );	// Wait for ready
	
	// Enable the digital feedback locked loop calibration for the 32MHz clock
	DFLLRC32M_CTRL = DFLL_ENABLE_bm;

	// Allow changing of CLK.CTRL
	CCP = CCP_IOREG_gc;	
	// Set system clock to 32MHz oscillator
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}