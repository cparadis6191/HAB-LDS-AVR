#include "state_machine.h"
#include "math.h"

static FILE PC_STREAM = FDEV_SETUP_STREAM(USARTC0_putchar, USARTC0_getchar, _FDEV_SETUP_RW);
static FILE LCD_STREAM = FDEV_SETUP_STREAM(USARTC1_putchar, USARTC1_getchar, _FDEV_SETUP_RW);

// Global variable to hold the current state of the system
volatile int ST_STATE = ST_INIT;

void state_machine(void) {
	int count = 0;
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

				// Initialize communication to the PC
				USARTC0_init();
				
				// Initialize communication to the LCD
				USARTC1_init();
				fprintf(&PC_STREAM, "IN ST_INIT\r\n");

				// Configure the ADC
				adc_init();

				// Initialize the LCD
				
				fprintf(&PC_STREAM, "BEFORE LCD_INIT\r\n");
				lcd_init();
				//lcd_backlight_on();
				fprintf(&PC_STREAM, "AFTER LCD_INIT\r\n");

				// Enable interrupts
				main_interrupts_init();
				// Enable global interrupts
				sei();

				// After initialization go to idle state
				ST_STATE = ST_IDLE;

				break;

			case ST_IDLE:
				ST_STATE = ST_POLLING_INIT;
			
				break;

			case ST_POLLING_INIT:
				fprintf(&PC_STREAM, "IN ST_POLLING_INIT\r\n");
				// Initialize EEPROM

				// Initialize a channel
				adc_channel_init();
				// Initialize adc interrupts
				adc_interrupt_init();
				// Enable the real-time clock and use it to generate interrupts every t seconds
				adc_timer_init(1);

				// Skip the if statement in ST_POLLING the first time
				g_ADC_CONVERSION_COMPLETE_CHANNEL_0 = 1;
				g_ADC_CONVERSION_COMPLETE_CHANNEL_1 = 1;
				
				// After things are initialized
				ST_STATE = ST_POLLING;

				break;

			case ST_POLLING:
				fprintf(&PC_STREAM, "IN ST_POLLING\r\n");
				while (1) {
					// Iterate through each sensor and record data
					// Use two channels to get data from
					// Start at 1 because PA0 is a reference
					for (g_ADC_INDEX = 1; g_ADC_INDEX <= 10; g_ADC_INDEX++) {
						// Starts a conversion on index n and n + 5
						adc_start(g_ADC_INDEX, g_ADC_INDEX + 5);

						// Wait until the ISRs handle each ADC channel
						if ((g_ADC_CONVERSION_COMPLETE_CHANNEL_0 && g_ADC_CONVERSION_COMPLETE_CHANNEL_1)) {
							// Reset the conversion complete flags
							g_ADC_CONVERSION_COMPLETE_CHANNEL_0 = 0;
							g_ADC_CONVERSION_COMPLETE_CHANNEL_1 = 0;
						}
					}						

					// Write the recorded angle to memory
					if (g_ADC_RECORD_FLAG) {
						count++;
						// Sum the x and y components of the vectors
						lcd_clear_display();
						//fprintf(&LCD_STREAM, "PA%i is %i", count, g_ADC_RESULT[0]);
						//fprintf(&LCD_STREAM, "PA%i is %i", g_ADC_INDEX, g_ADC_RESULT[0]);
						fprintf(&LCD_STREAM, "Angle: %f", resolve_angle());
						
						g_ADC_RECORD_FLAG = 0;
					}
				}

				break;

			case ST_POLLING_DONE:
				fprintf(&PC_STREAM, "IN ST_POLLING_DONE\r\n");

				// Return to the idle state after pin is added back
				ST_STATE = ST_IDLE;
				
				break;

			case ST_PC_INIT_COMM:
				fprintf(&PC_STREAM, "IN ST_INIT_COMM\r\n");
			
				break;

			case ST_PC_CONNECTED:
				fprintf(&PC_STREAM, "IN ST_CONNECTED\r\n");
			
				break;

			case ST_PC_SEND_DATA:
				fprintf(&PC_STREAM, "IN ST_PC_SEND_DATA\r\n");
			
				break;

			case ST_PC_RECEIVE_SETTINGS:
				fprintf(&PC_STREAM, "IN ST_PC_RECEIVE_SETTINGS\r\n");
			
				break;
			
			case ST_PC_DISCONNECT:
				fprintf(&PC_STREAM, "IN ST_PC_DISCONNECT\r\n");
			
				break;
			default:
				fprintf(&PC_STREAM, "IN ST_DEFAULT\r\n");

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

double resolve_angle(void) {
	// Arrays holding the sensor x/y-component magnitudes
	// Values calculated with Matlab sin(pi/180*(0:9)*36) and cos(pi/180*(0:9)*36)
	const double sensor_x_component[10] = {1.0000, 0.8090, 0.3090, -0.3090, -0.8090};
	const double sensor_y_component[10] = {0, 0.5878, 0.9511, 0.9511, 0.5878};

	double interm_x = 0;
	double interm_y = 0;
	
	for (int i = 0; i < 5; i++) {
		// For each sensor, subtract opposite sensor and determine x/y-components
		interm_x += sensor_x_component[i]*(g_ADC_RESULT[i] - g_ADC_RESULT[i + 5]);
		interm_y += sensor_y_component[i]*(g_ADC_RESULT[i] - g_ADC_RESULT[i + 5]);
	}

	// Resolve the angle using arctangent and convert to an angle in degrees
	return (double) atan2(interm_y, interm_x)*(180/M_PI);
}

// Catch all unhandled interrupt vectors
ISR(BADISR_vect) {

	return;
}