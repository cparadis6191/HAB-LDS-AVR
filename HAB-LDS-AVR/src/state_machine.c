#include "state_machine.h"

static FILE PC_STREAM = FDEV_SETUP_STREAM(USARTC0_putchar, USARTC0_getchar, _FDEV_SETUP_RW);

volatile int ST_STATE = ST_INIT;

void state_machine(void) {
	while (1) {
		switch (ST_STATE) {
			// Initialization state
			// Initialize the clocks
			// Initialize interrupts
			// Go to the idle state after initialization
			case ST_INIT:

				// Initialize the clocks				
				// Initialize the main clock
				clock_32MHz_init();

				// Initialize the real-time clock
				clock_32kHz_init();

				// Initialize communication to the LCD
				USARTC0_init();
				
				// Configure the ADC
				adc_init();
	
				// Enable interrupts
				main_interrupts_init();
				// Enable global interrupts
				sei();

				// After initialization go to idle state
				ST_STATE = ST_IDLE;

				continue;

			case ST_IDLE:
				// Wait until PC connects or polling pin is pulled off

				ST_STATE = ST_POLLING_INIT;
			
				continue;

			case ST_POLLING_INIT:
				// Initialize EEPROM
				// Enable the real-time clock and use it to generate interrupts
				adc_timer_init();
				
				adc_channel_init();
				// Initialize adc interrupts
				adc_interrupt_init();
				
				// After things are initialized
				ST_STATE = ST_POLLING;

				continue;

			case ST_POLLING:
				while (1) {
					g_ADC_INDEX = 4;
					if (g_ADC_POLL_FLAG) {
						fprintf(&PC_STREAM, "%i\r\n", g_ADC_RESULT[g_ADC_INDEX]);
						
						g_ADC_POLL_FLAG = 0;
						
						adc_start(g_ADC_INDEX);
					}
				}

				continue;

			case ST_POLLING_DONE:

				// Return to the idle state after pin is added back
				ST_STATE = ST_IDLE;
				
				continue;

			case ST_PC_INIT_COMM:
			
				continue;

			case ST_PC_CONNECTED:
			
				continue;

			case ST_PC_SEND_DATA:
			
				continue;

			case ST_PC_RECEIVE_SETTINGS:
			
				continue;
			
			case ST_PC_DISCONNECT:
			
				continue;
			default:

				continue;
		}
	}
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

void main_interrupts_init(void) {
	// Enable low/mid/high level global interrupts
	PMIC.CTRL = (PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm);

	return;
}