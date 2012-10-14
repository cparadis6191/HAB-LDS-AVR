#include "HAB-LDS-AVR.h"

static FILE myio = FDEV_SETUP_STREAM(USARTC0_putchar, USARTC0_getchar, _FDEV_SETUP_RW);
<<<<<<< HEAD
volatile ADC_POLL_FLAG;
=======
>>>>>>> 1db710ab314aaf82ac93c1f184df51ca21d3023e

int main(void) {

	//state_machine();
	clock_init();
	USARTC0_init();
	lcd_init();
	
	stdout = stdin = &myio;
	
<<<<<<< HEAD
	while (1) {
	
		if (ADC_POLL_FLAG) {
			printf("yay\r\n");
		}
		
=======
	char temp = "";
	while( 1 ) {
		scanf("%c", temp);
		printf("%c", temp);
>>>>>>> 1db710ab314aaf82ac93c1f184df51ca21d3023e
	}

	return 0;
}

// Set the chip to run on the 32MHz internal oscillator
void clock_init(void) {
	// Allow changing of OSC.CTRL
	CCP = CCP_IOREG_gc;	

	// Start 32MHz RC oscillator (main system clock) and 32.875kHz oscillator (as a reference in calibration for the main clock)
	OSC.CTRL |= (OSC_RC32MEN_bm | OSC_RC32KEN_bm);

	// Wait for both clocks to be ready
	while (((OSC.STATUS & (OSC_RC32MRDY_bm | OSC_RC32KRDY_bm))) != (OSC_RC32MRDY_bm | OSC_RC32KRDY_bm));

	// Enable the digital feedback locked loop calibration for the 32MHz clock
	// Allow changing of CLK.CTRL
	CCP = CCP_IOREG_gc;	

	// Set system clock to 32MHz oscillator
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;

	// Clock is running at 33MHz... trying to figure out why, enabling the 32kHz clock should fix it
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;
	
	return;
}

void interrupts_init(void) {
	// Enable low/mid/high level global interrupts
	PMIC.CTRL |= (PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm);
	
	return;
}