#include "state_machine.h"

// Peripheral streams to print to
static FILE LCD_STREAM = FDEV_SETUP_STREAM(USARTD0_putchar, USARTD0_getchar, _FDEV_SETUP_RW);
static FILE PC_STREAM = FDEV_SETUP_STREAM(USARTD1_putchar, USARTD1_getchar, _FDEV_SETUP_RW);

// Global variable to hold the current state of the system
volatile int ST_STATE = ST_INIT;

void state_machine(void) {
	// Variable to hold the last location of data in EEPROM
	int MEM_LOC = eeprom_read_word((void *) EEPROM_LAST_DATA);
	int POLLING_INT = eeprom_read_byte((void *) EEPROM_POLLING_INTERVAL);

	while (1) {
		switch (ST_STATE) {
			// Initialization state
			// Go to the idle state after initialization
			case ST_INIT:
				// Initialize the clocks				
				// Initialize the main clock
				clock_32MHz_init();
				// Initialize the real-time clock
				clock_32kHz_init();
				// Initialize DFLL to calibrate the 32MHz clock off of the 32kHz clock
				clock_DFLL_init();
				
				// Initialize communication to the LCD
				USARTD0_init();
				// Initialize communication to the PC
				USARTD1_init();

				// Configure the ADC
				adc_init();

				// Initialize the LCD
				lcd_init();

				// Initialize the jumper
				input_init();
				// Initialize the logic to determine when the FT232 chip is connected to a PC
				pc_interface_init();

				// Enable interrupts
				main_interrupts_init();
				// Enable global interrupts
				sei();

				// After initialization go to idle state
				ST_STATE = ST_IDLE;

				break;

			case ST_IDLE:
				// If the jumper is removed,
				if (!(PORTC.IN & PIN0_bm)) {
					ST_STATE = ST_POLLING_INIT;
				// Else check if the PC is connected
				} else if (PORTC.IN & PIN2_bm) {
					ST_STATE = ST_PC_INIT_COMM;
				}
			
				break;

			case ST_POLLING_INIT:
				// Initialize EEPROM

				// Initialize a channel
				adc_channel_init();
				// Initialize adc interrupts
				adc_interrupt_init();
				// Enable the real-time clock and use it to generate interrupts every t seconds
				//adc_timer_init(POLLING_INT);
				adc_timer_init(1);

				// Skip the if statement in ST_POLLING the first time
				g_ADC_CH0_COMPLETE = 0;
				g_ADC_CH1_COMPLETE = 0;
				g_ADC_RECORD_FLAG = 0;
				
				// After things are initialized
				ST_STATE = ST_POLLING;

				break;

			case ST_POLLING:
				// Poll the sensors until the jumper is added back
				while (!(PORTC.IN & PIN0_bm)) {
					double sensor_results[10];
					double current_angle;

					// Write the recorded angle to memory
					if (g_ADC_RECORD_FLAG) {
						// Iterate through each sensor and record data
						// Use two channels to get data from
						// Start at 1 because PA0 is a reference
						for (int sensor_index = 1; sensor_index <= 5; sensor_index++) {
								sensor_results[sensor_index - 1] = 0;
								sensor_results[sensor_index + 5 - 1] = 0;

							// Do four conversions and average the results
							for (int sensor_sum_num = 0; sensor_sum_num <= 4; sensor_sum_num++) {
								// Starts a conversion on index n and n + 5
								adc_start(sensor_index, sensor_index + 5);

								// Wait until the ISRs handle each ADC channel
								if ((g_ADC_CH0_COMPLETE && g_ADC_CH1_COMPLETE)) {
									// Reset the conversion complete flags
									g_ADC_CH0_COMPLETE = 0;
									g_ADC_CH1_COMPLETE = 0;

									// Sum the results
									sensor_results[sensor_index - 1] += (g_ADC_CH0_RESULT - 180)/4.0;
									sensor_results[sensor_index + 5 - 1] += (g_ADC_CH1_RESULT - 180)/4.0;
								}
							}
						}

						current_angle = resolve_angle(sensor_results);

						lcd_clear_display();
						lcd_clear_display();
						fprintf(&LCD_STREAM, "Ang: %.1f", current_angle);
						//fprintf(&LCD_STREAM, "ADC: %.1f", sensor_results[0]);

						if (MEM_LOC <= (EEPROM_END - 1)) {
							// Write the data to EEPROM
							// Multiply by ten and store the result as an int so we can use only two bytes of storage
							eeprom_write_word((void *) MEM_LOC, (int) (current_angle*10));
							// Write to EEPROM and store the address of the latest angle
							eeprom_write_word((void *) EEPROM_LAST_DATA, MEM_LOC);

							// Increment to point to the next data point
							MEM_LOC += 2;
						}

						// Reset the record flag and start polling again
						g_ADC_RECORD_FLAG = 0;
					}
				}

				ST_STATE = ST_POLLING_DONE;

				break;

			case ST_POLLING_DONE:
				// Return to the idle state after pin is added back
				ST_STATE = ST_IDLE;
				
				break;

			case ST_PC_INIT_COMM:
				// Wait for the PC_INIT byte
				while(fgetc(&PC_STREAM) != PC_INIT);

				// Send the acknowledgment
				fputc(AVR_ACK, &PC_STREAM);

				// Go to the ST_PC_CONNECTED state after initialization
				ST_STATE = ST_PC_CONNECTED;

				break;

			case ST_PC_CONNECTED:
				// Get a command from the PC
				switch(fgetc(&PC_STREAM)) {
					case PC_DATA_REQUEST:
						// Transmit data until the end of EEPROM or until the end of recorded memory
						for (int i = EEPROM_DATA_START; i < MEM_LOC; i += 2) {
							fprintf(&PC_STREAM, "%.1f\n", (int) eeprom_read_word((void *) i)/10.0);
						}

						// Send end of data signal
						fputc(AVR_DATA_END, &PC_STREAM);

						break;

					case PC_POLLING_INTERVAL:
						// Get the polling interval from the PC
						POLLING_INT = fgetc(&PC_STREAM);

						// Store the polling interval to EEPROM
						eeprom_write_byte((void *) EEPROM_POLLING_INTERVAL, POLLING_INT);

						break;

					case PC_ERASE_DATA:
						// Reset the EEPROM location to 2
						MEM_LOC = EEPROM_DATA_START;

						// Write it to EEPROM
						eeprom_write_word((void *) EEPROM_LAST_DATA, MEM_LOC);

						break;

					case PC_COMM_CLOSE:
						ST_STATE = ST_PC_DISCONNECT;

						break;

					default:

						break;
				}

				break;

			case ST_PC_DISCONNECT:
				// Wait for the cable to be disconnected from the FT232
				while (PORTC.IN & PIN2_bm);

				ST_STATE = ST_IDLE;
			
				break;
			default:

				break;
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

	return;
}

void clock_32kHz_init( void ) {
	// Allow changing of OSC.CTRL
	CCP = CCP_IOREG_gc;
	// Start 32kHz RC oscillator (main system clock)
	OSC.CTRL |= (OSC_RC32KEN_bm);

	// Wait until the 32kHz clock is ready
	while (!(OSC.STATUS & OSC_RC32KRDY_bm));

	// Allow changing of CLK.CTRL
	CCP = CCP_IOREG_gc;
	// select RTC clock source, 32.768kHz
	CLK.RTCCTRL = (CLK_RTCSRC_RCOSC32_gc | CLK_RTCEN_bm);

	return;
}

void clock_DFLL_init(void) {
	// Set the DFLL source to the 32kHz RTC
	CLK.DFLLCTRL = OSC_RC32MCREF_RC32K_gc;

	// Enable the digital feedback locked loop calibration for the 32MHz clock
	// The 32kHz clock also needs to be enabled for this to work
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;

	return;
}

void main_interrupts_init(void) {
	// Enable low/mid/high level global interrupts
	PMIC.CTRL = PMIC_LOLVLEN_bm;

	return;
}

void input_init(void) {
	// Set the jumper pin as an input
	PORTC.DIRCLR |= PIN0_bm;
	// Set PC0 to pulldown and sense rising edge signals
	PORTC.PIN0CTRL = PORT_OPC_PULLDOWN_gc;

	return;
}

// Pass in a pointer to an array of length 10
double resolve_angle(double *sensor_results) {
	// Arrays holding the sensor x/y-component magnitudes
	// Values calculated with Matlab cos(pi/180*(0:4)*36) and sin(pi/180*(0:4)*36)
	const double sensor_x_coeff[5] = {1.0, 0.8090, 0.3090, -0.3090, -0.8090};
	const double sensor_y_coeff[5] = {0.0, 0.5878, 0.9511, 0.9511, 0.5878};

	double interm_x = 0;
	double interm_y = 0;
	
	for (int i = 0; i < 5; i++) {
		// For each sensor, subtract opposite sensor and determine x/y-components
		interm_x += sensor_x_coeff[i]*(sensor_results[i] - sensor_results[i + 5]);
		interm_y += sensor_y_coeff[i]*(sensor_results[i] - sensor_results[i + 5]);
	}

	// Resolve the angle using arctangent and convert to an angle in degrees
	return atan2(interm_y, interm_x)*(180/M_PI);
}

// Catch all unhandled interrupt vectors
ISR(BADISR_vect) {

	return;
}