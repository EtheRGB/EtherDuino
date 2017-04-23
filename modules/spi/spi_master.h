/*! @brief SPI Master-mode definitions
 *
 *	@author	inselc
 *	@date	05.12.16	initial version	
 *	@date	23.04.17	Added getSpiClkRate							*/ 

#ifndef SPI_MASTER_H_
#define SPI_MASTER_H_

/*! @file */

#include "spi_common.h"

/*!	@enum spiClkRate_t
 *	SPI Clock divider rate selector									*/
typedef enum tagSpiClkRate_t 
{
	SPI_FOSC_DIV_2  = 0x04,
	SPI_FOSC_DIV_4  = 0x00,
	SPI_FOSC_DIV_8  = 0x05,
	SPI_FOSC_DIV_16 = 0x01,
	SPI_FOSC_DIV_32 = 0x06,
	SPI_FOSC_DIV_64 = 0x02,
	SPI_FOSC_DIV_128= 0x03
} spiClkRate_t;

/*! @brief Set SPI Clock rate in Master mode
 *
 *	@param[in] rate			SPI Clock rate
 *	@date 05.12.16			first implementation					*/
static inline void spiSetMstrClkRate(spiClkRate_t rate)
{
	// SPI Double Speed bit
	SPSR &= ~((((~rate)&0x04) >> 2) << SPI2X);
	SPSR |= ((rate&0x04) >> 2) << SPI2X;

	// SPI Clock Rate Select
	SPCR &= ~(((~rate)&0x03) << SPR0);
	SPCR |= (rate&0x03) << SPR0;
}

/*!	@brief Get current SPI Clock rate configuration 
 *
 *	@return spiClkRate_t	SPI Clock rate
 *	@date 23.04.17			First implementation					*/
static inline spiClkRate_t spiGetMstrClkRate(void)
{
	spiClkRate_t rate = (SPCR >> SPR0) & 0x03;
	rate |= ((SPSR >> SPI2X) & 0x01) << 2;

	return rate;
}

void spiInitMaster(spiClkRate_t clkRate, spiClkMode_t clkMode, spiDataOrder_t dataOrder);

#endif /* SPI_MASTER_H_ */