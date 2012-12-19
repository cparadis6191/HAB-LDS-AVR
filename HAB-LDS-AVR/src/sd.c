#include "sd.h"


void sd_init_spi(void) {
	// Sets the AVR to be the master
	// Need to read about SPI_DORD and SPI_MODE
	// Prescale down to below 400kHz so the SD can be communicated with before initialization
	SPIC.CTRL = (SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESCALER_DIV128_gc);


	return;
}