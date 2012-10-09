#include "adc.h"

void init_adc( void ) {
	// Enable the Analog-to-Digital converter
	ADCA.CTRLA |= (1 << ADC_ENABLE_bp);

	// 
	ADCA.CTRLA |= (1 << ADC_ENABLE_bp);

	// Set the resolution of the ADC to be 8-bit, right adjusted
	ADCA.CTRLB |= (1 << ADC_RESOLUTION1_bp)
	ADCA.CTRLB &= ~(1 << ADC_RESOLUTION0_bp);

	return;
}
