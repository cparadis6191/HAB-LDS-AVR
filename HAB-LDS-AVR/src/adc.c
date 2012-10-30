#include "adc.h"

void adc_init(void) {
	// Set the resolution of the ADC to be 12-bit, right-adjusted
	// Unsigned by default
	// read low ADCA calibration byte from NVM signature row into register
	 ADCA.CALL = adc_read_calibration_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
	 // read high ADCA calibration byte from NVM signature row into register
	 ADCA.CALH = adc_read_calibration_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));

	// Set the reference to be AREFA on PA0
	ADCA.REFCTRL = ADC_REFSEL_AREFA_gc;

	// Enable the Analog-to-Digital converter
	ADCA.CTRLA |= ADC_ENABLE_bm;

	return;
}

void adc_channel_init(void) {
	// Set ADC CH0 to run in single ended mode with a gain of 1x
	ADCA.CH0.CTRL = (ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc);
	ADCA.CH1.CTRL = (ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc);
	
	return;
}

void adc_interrupt_init(void) {
	// Set ADC CH0 to trigger a low-level interrupt when a conversion completes
	ADCA.CH0.INTCTRL = (ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc);
	ADCA.CH1.INTCTRL = (ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc);
	
	return;
}

void adc_start(int channel_0_pin, int channel_1_pin) {
	// Choose the pin for channel 1
	ADCA.CH0.MUXCTRL = (channel_0_pin << 3);
	// Choose the pin for channel 1
	ADCA.CH1.MUXCTRL = (channel_1_pin << 3);
	
	// Start a conversion on both channels
	ADCA.CH0.CTRL |= ADC_CH_START_bm;
	ADCA.CH1.CTRL |= ADC_CH_START_bm;

	return;
}

// Set up the timer to produce interrupts every t seconds
void adc_timer_init(int t) {
	// Prescaler of 64
	RTC.CTRL = RTC_PRESCALER_DIV256_gc;
	
	// Top value of counter
	// t = 15 (delay in seconds)
	// top = 32.768kHz/prescaler*t
	int top = 128*t;

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

ISR(ADCA_CH0_vect) {
	// Results are 12-bit right-adjusted
	// AVRs have 16-bit integers

	// Store the lower eight bits
	g_ADC_RESULT_CHANNEL_0 = ADCA.CH0.RESL;

	// Store the upper four bits
	g_ADC_RESULT_CHANNEL_0 += (ADCA.CH0.RESH << 8);
	
	// Mark the conversion as complete
	g_ADC_CONVERSION_COMPLETE_CHANNEL_0 = 1;
	
	return;
}

ISR(ADCA_CH1_vect) {
	// Results are 12-bit right-adjusted
	// AVRs have 16-bit integers

	// Store the lower eight bits
	g_ADC_RESULT_CHANNEL_1 = ADCA.CH1.RESL;

	// Store the upper four bits
	g_ADC_RESULT_CHANNEL_1 += (ADCA.CH1.RESH << 8);

	// Mark the conversion as complete
	g_ADC_CONVERSION_COMPLETE_CHANNEL_1 = 1;
	
	return;
}

// Counter overflow interrupt vector for the real-time clock
ISR(RTC_OVF_vect) {
	// After t seconds, record and display the data
	g_ADC_RECORD_FLAG = 1;

	return;
}

uint8_t adc_read_calibration_byte(uint8_t address) {
	uint8_t result;

	// Load the NVM Command register to read the calibration row
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	result = pgm_read_byte(address);

	// Clean up NVM Command register
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;

	return result;
}