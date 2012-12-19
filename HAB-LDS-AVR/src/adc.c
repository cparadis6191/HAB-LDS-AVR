#include "adc.h"


void adc_init(void) {
	// Set the resolution of the ADC to be 12-bit, right-adjusted
	// Unsigned by default

	// Set the reference to be AREFA on PA0
	ADCA.REFCTRL = (ADC_REFSEL_AREFA_gc | ADC_BANDGAP_bm);

	// Set the prescaler to be 32
	// Changing from this prescaler (either higher or lower) will cause the ADC to not work
	// Prescaler of 128 brings the ADC clock down to 250kHz
	ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;

	// read low ADCA calibration byte from NVM signature row into register
	ADCA.CALL = adc_read_calibration_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
	// read high ADCA calibration byte from NVM signature row into register
	ADCA.CALH = adc_read_calibration_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));

	// Enable the Analog-to-Digital converter
	ADCA.CTRLA = ADC_ENABLE_bm;


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
	RTC.PER = top;

	// Enable real-time counter overflow interrupts
	RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;

	// Set the counter to 0
	RTC.CNT = 0;


	return;
}

ISR(ADCA_CH0_vect) {
	// Results are 12-bit right-adjusted
	// AVRs have 16-bit integers

	// Store the result back (taken from two registers)
	g_ADC_CH0_RESULT = ADCA.CH0.RES;

	// Mark the conversion as complete
	g_ADC_CH0_COMPLETE = 1;
	
	
	return;
}

ISR(ADCA_CH1_vect) {
	// Results are 12-bit right-adjusted
	// AVRs have 16-bit integers

	// Store the result back (taken from two registers)
	g_ADC_CH1_RESULT = ADCA.CH1.RES;

	// Mark the conversion as complete
	g_ADC_CH1_COMPLETE = 1;
	
	
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


void adc_poll_sensors(int* sensor_readings, int* CH0_pin_num, int* CH1_pin_num) {
	// Iterate through each sensor and record data
	// Use two channels to get data from
	// Start at 1 because PA0 is a reference
	for (int sensor_index = 1; sensor_index <= 5; sensor_index++) {
			sensor_readings[sensor_index - 1] = 0;
			sensor_readings[sensor_index + 5 - 1] = 0;

		// Do four conversions and average the readings
		for (int sensor_sum_num = 0; sensor_sum_num < 50; sensor_sum_num++) {
			// Starts a conversion on index n and n + 5
			adc_start(CH0_pin_num[sensor_index - 1], CH1_pin_num[sensor_index - 1]);

			// Wait until the ISRs handle each ADC channel
			if ((g_ADC_CH0_COMPLETE && g_ADC_CH1_COMPLETE)) {
				// Reset the conversion complete flags
				g_ADC_CH0_COMPLETE = 0;
				g_ADC_CH1_COMPLETE = 0;

				// Sum the readings
				sensor_readings[sensor_index - 1] += (g_ADC_CH0_RESULT)/50.0;
				sensor_readings[sensor_index + 5 - 1] += (g_ADC_CH1_RESULT)/50.0;
			}
		}
	}
	
	
	return;
}


// Counter overflow interrupt vector for the real-time clock
ISR(RTC_OVF_vect) {
	// After t seconds, record and display the data
	g_ADC_RECORD_FLAG = 1;


	return;
}
