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

#include "globals.h"
volatile int ADC_POLL_FLAG;
ISR(RTC_OVF_vect) {
	ADC_POLL_FLAG = 1;

	return;
}

void init_timer(void) {
	// Prescaler of 64
	RTC.CTRL = RTC_PRESCALER_DIV64_gc;
	
	// Top value of counter
	// t = 15
	// 32.768kHz/64*t
	// 512*t
	RTC.PERL = (uint8_t) 7689;
	RTC.PERH = (7689 >> 8);

	// Enable real-time counter overflow interrupts
	RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;

	return;
}