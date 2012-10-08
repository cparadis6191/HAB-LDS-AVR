#include "avr/io.h"

#include "light_direction_sensor.h"
#include "bluetooth.h"
#include "lcd.h"
#include "sd.h"

int main( void ) {

	init_test();


	return 0;
}


void init_adc( void ) {
	

}

// Light an LED
void init_test( void ) {

	// Set pin PR0 to an output in PORTR
	PORTR.DIR &= 0b00000001;

	// Set PR1 to have a pullup resistor
	PORTR.PIN1CTRL |= 0b00011000;

	// Set pin PR1 to an input in PORTR
	PORTR.DIR |= 0b00000010;

	PORTR.OUT |= (PORTR.IN & (1 << 1));
}
