#include "HAB-LDS-AVR.h"

static FILE myio = FDEV_SETUP_STREAM(USARTC0_putchar, USARTC0_getchar, _FDEV_SETUP_RW);

int main(void) {

	//state_machine();
	clock_32kHz_init();
	clock_32MHz_init();
	USARTC0_init();
	timer_init();
	interrupts_init();
	sei();

	stdout = stdin = &myio;
	int i = 0;
	
	while (1) {
	
		if (ADC_POLL_FLAG) {
			i++;
			printf("%i\r\n", i);
			
			ADC_POLL_FLAG = 0;
		}
		
	}

	return 0;
}

// Set the chip to run on the 32MHz internal oscillator
void clock_32MHz_init(void) {
	// Allow changing of OSC.CTRL
	CCP = CCP_IOREG_gc;
	// Start 32MHz RC oscillator (main system clock)
	OSC.CTRL |= (OSC_RC32MEN_bm);

	// Wait for 32MHz clock to be ready
	while (!(OSC.STATUS & OSC_RC32MRDY_bm));

	// Allow changing of CLK.CTRL
	CCP = CCP_IOREG_gc;

	// Set system clock to 32MHz oscillator
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;

	// Enable the digital feedback locked loop calibration for the 32MHz clock
	// The 32kHz clock also needs to be enabled for this to work
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;

	return;
}

void clock_32kHz_init( void ) {
	// Allow changing of OSC.CTRL
	CCP = CCP_IOREG_gc;
	// Start 32kHz RC oscillator (main system clock)
	OSC.CTRL |= (OSC_RC32KEN_bm);

	// Wait until the 32kHz clock is ready
	while (!(OSC.STATUS & OSC_RC32KRDY_bm));

	// select RTC clock source, 32.768kHz
	CLK.RTCCTRL = (CLK_RTCSRC_RCOSC32_gc | CLK_RTCEN_bm);

	// wait for RTC SYNC status not busy before returning
	while ((RTC.STATUS & RTC_SYNCBUSY_bm));

	return;
}

void interrupts_init(void) {
	// Enable low/mid/high level global interrupts
	PMIC.CTRL = (PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm);

	return;
}
