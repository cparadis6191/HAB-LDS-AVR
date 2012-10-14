#include "adc.h"

void init_adc(void) {
	// Enable the Analog-to-Digital converter
	ADCA.CTRLA |= ADC_ENABLE_bm;

	// Set the resolution of the ADC to be 8-bit, right adjusted
	ADCA.CTRLB |= ADC_RESOLUTION_12BIT_gc;

	return;
}

void init_adc_interrupts(void) {
	
	
	return;
}

ISR(ADCA_CH0_vect) {
	ADCA.CH0.RESH;
	ADCA.CH0.RESL;
	
	return;
}

ISR(RTC_OVF_vect) {
	ADC_POLL_FLAG = 1;

	return;
}

void timer_init(void) {
	// Prescaler of 64
	RTC.CTRL = RTC_PRESCALER_DIV2_gc;
	
	// Top value of counter
	// t = 15 (delay in seconds)
	// top = 32.768kHz/prescaler*t
	int top = 0;

	RTC.PERL = (uint8_t) top;
	RTC.PERH = (top >> 8);

	// Enable real-time counter overflow interrupts
	RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;

	return;
}