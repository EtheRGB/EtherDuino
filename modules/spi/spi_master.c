/*
 * spi_master.c
 *
 * Created: 12/10/2016 10:05:17 PM
 *  Author: inselc
 */ 

#include <avr/io.h>
#include "spi_master.h"

/*! @brief Initialise SPI in Master mode
 *
 *	@param[in] clkRate		Clock speed
 *	@param[in] clkMode		Clock mode (0...3)
 *	@param[in] dataOrder	Data order (MSB or LSB first)
 *	@date 05.12.16			first implementation					*/
void spiInitMaster(spiClkRate_t clkRate, spiClkMode_t clkMode, spiDataOrder_t dataOrder)
{
	// Enable SPI module power
	spiPowerEnable();

	// make sure SPI is disabled for setup
	spiDisable();

	// configure as SPI Master
	spiSetMstrSlave(SPI_MASTER);

	// set SPI data order
	spiSetDataOrder(dataOrder);

	// apply clock configuration
	spiSetClkMode(clkMode);
	spiSetMstrClkRate(clkRate);
	
	// Set PB5(SCK) abd PB3 (MOSI) as outputs
	DDRB |= (1 << 5) | (1 << 3);
	// Disable pull-up on PB4 (MISO)
	PORTB &= ~(1 << 4);
	// Set PB4 (MISO) as input
	DDRB &= ~(1 << 4);

	// enable SPI
	spiEnable();
}