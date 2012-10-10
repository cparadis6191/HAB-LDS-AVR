#include "adc.h"

void init_adc( void ) {
	// Enable the Analog-to-Digital converter
	ADCA.CTRLA |= ADC_ENABLE_bm;

	// Set the resolution of the ADC to be 8-bit, right adjusted
	ADCA.CTRLB |= ADC_RESOLUTION_8BIT_gc;

	return;
}
