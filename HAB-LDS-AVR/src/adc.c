#include "adc.h"

void adc_init(void) {
	// Set the resolution of the ADC to be 12-bit, right-adjusted
	// Unsigned by default
	//ADCA.CTRLA |= ;

	ADCA.CTRLB |= ADC_RESOLUTION_12BIT_gc;

	// Set ADC CH0 to run in single ended mode with a gain of 1x
	ADCA.CH0.CTRL |= (ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc);

	

	// Enable the Analog-to-Digital converter
	ADCA.CTRLA |= ADC_ENABLE_bm;

	return;
}

void init_adc_interrupts(void) {
	// Set ADC CH0 to trigger a low-level interrupt when a conversion completes
	ADCA.CH0.INTCTRL |= ( ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc);
	
	return;
}

ISR(ADCA_CH0_vect) {
	// Results are 12-bit right-adjusted
	// AVRs have 16-bit integers
	ADC_RESULT[ADC_INDEX] = 0;

	// Store the lower eight bits
	ADC_RESULT[ADC_INDEX] = ADCA.CH0.RESL;

	// Store the upper four bits
	ADC_RESULT[ADC_INDEX] = (ADCA.CH0.RESH << 8);
	
	return;
}

ISR(RTC_OVF_vect) {
	ADC_POLL_FLAG = 1;

	return;
}

void timer_init(void) {
	// Prescaler of 64
	RTC.CTRL = RTC_PRESCALER_DIV256_gc;
	
	// Top value of counter
	// t = 15 (delay in seconds)
	// top = 32.768kHz/prescaler*t
	int top = 128*1;

	// Wait until the clock is synced
	while ((RTC.STATUS & RTC_SYNCBUSY_bm));
	RTC.PERL = (uint8_t) top;

	while ((RTC.STATUS & RTC_SYNCBUSY_bm));
	RTC.PERH = (top >> 8);

	// Enable real-time counter overflow interrupts
	RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;

	// Set the counter to 0
	RTC.CNT = 0;

	return;
}