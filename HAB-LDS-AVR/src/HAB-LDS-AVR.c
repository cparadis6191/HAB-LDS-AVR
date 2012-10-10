#include "HAB-LDS-AVR.h"

int main( void ) {
	clock_init();
	lcd_init();
	
	while( 1 );

	return 0;
}

// Set the chip to run on the 32MHz internal oscillator
void clock_init( void ) {
	// Start 32MHz RC oscillator
	OSC.CTRL = OSC_RC32MEN_bm;
	
	// Wait for ready
	while ( !(OSC.STATUS & OSC_RC32MRDY_bm) );	// Wait for ready
	
	// Allow changing of CLK.CTRL
	CCP = CCP_IOREG_gc;
	
	// Set system clock to 32MHz oscillator
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}