/*! @brief Common SPI definitions
 *
 *	@author	inselc
 *	@date	05.12.16	initial version								*/ 


#ifndef SPI_COMMON_H_
#define SPI_COMMON_H_

#include <stdbool.h>
#include <stdint.h>

/*! @file */

/*! @enum spiMode_t
 *	SPI Master/Slave mode configuration								*/
typedef enum tagSpiMode_t 
{
	SPI_SLAVE = 0,
	SPI_MASTER = 1
} spiMode_t;

/*! @enum spiDataOrder_t
 *	SPI Data Order mode												*/
typedef enum tagSpiDataOrder_t 
{
	SPI_MSBFIRST = 0,
	SPI_LSBFIRST = 1
} spiDataOrder_t;

/*! @enum spiClkMode_t
 *	SPI Clock Mode selector											*/
typedef enum tagSpiClkMode_t
{
	SPI_MODE0 = 0x00,
	SPI_MODE1 = 0x01,
	SPI_MODE2 = 0x02,
	SPI_MODE3 = 0x03
} spiClkMode_t;

/*! @brief Enable SPI module power
 *
 *	@date 04.12.16			first implementation					*/
static inline void spiPowerEnable(void)
{
	// Enable SPI module in the power reduction register as per
	// datasheet p. 215 (23.2 Overview)
	PRR &= ~(1 << PRSPI);
}

/*! @brief Disable SPI module power
 *
 *	@date 04.12.16			first implementation					*/
static inline void spiPowerDisable(void)
{
	// Disable SPI module in the power reduction register as per
	// datasheet p. 215 (23.2 Overview)
	PRR |= 1 << PRSPI;
}

/*! @brief Enable SPI function
 *
 *	@date 05.12.16			first implementation					*/
static inline void spiEnable(void)
{
	SPCR |= 1 << SPE;
}

/*! @brief Disable SPI function
 *
 *	@date 05.12.16			first implementation					*/
static inline void spiDisable(void)
{
	SPCR &= ~(1 << SPE);
}

/*! @brief Select SPI Slave or Master configuration
 *
 *	@param[in] mode			Slave/Master mode
 *	@date 05.12.16			first implementation					*/
static inline void spiSetMstrSlave(spiMode_t mode)
{
	// Set/Clear SPI master mode in SPCR as per datasheet 
	// p. 221 (23.5.1 SPI Control Register 0)
	SPCR &= ~(!mode << MSTR);
	SPCR |= mode << MSTR;
}

/*! @brief Apply SPI Clock mode configuration (polarity and phase)
 *
 *	See Datasheet p. 221f for information on SPI modes.
 *
 *	@param[in] mode			SPI Mode (0...3)
 *	@date 05.12.16			first implementation					*/
static inline void spiSetClkMode(spiClkMode_t mode)
{
	// Set/Clear SPI clock polarity and phase as per datasheet
	// p. 221f (23.5.1 SPI Control Register 0)
	SPCR &= ~((~mode)&0x03 << CPHA);
	SPCR |= mode << CPHA;
}

/*!	@brief Setup SPI data order
 *
 *	@param[in] order		Data direction LSB or MSB first
 *	@date 05.12.16			first implementation					*/
static inline void spiSetDataOrder(spiDataOrder_t order)
{
	SPCR &= ~(!order << DORD);
	SPCR |= order << DORD;
}

/*! @brief Get SPI interrupt flag state
 *
 *	@date 05.12.16			first implementation					*/
static inline bool spiGetIF(void)
{
	// Get interrupt flag state
	// see datasheet p. 223 (25.3.2 SPI Status Register 0)
	return (bool)(SPSR & (1 << SPIF));
}

/*! @brief Clear SPI interrupt flag
 *
 *  Manual SPIF clear. The flag will get cleared automatically,
 *  when first reading SPSR with SPIF set, and then accessing
 *  SPDR (see datasheet p. 223, 23.5.2 SPI Status Register 0).
 *
 *	@date 05.12.16			first implementation					*/
static inline void spiClrIF(void)
{
	// Clear interrupt flag
	SPSR &= (1 << SPIF);
}

/*! @brief Send byte over SPI
 *
 *	@param[in] data			Byte to be transmitted
 *	@date 05.12.16			first implementation					*/
static inline void spiTxByte(uint8_t data)
{
	SPDR = data;
}

/*! @brief Send byte over SPI and wait for SPIF
 *
 *	@param[in] data			Byte to be transmitted
 *	@date 07.12.16			first implementation					*/
static inline void spiTxByteWait(uint8_t data)
{
	spiTxByte(data);

	while(!spiGetIF()){;}
}

/*! @brief Read byte from SPI
 *
 *	@return	uint8_t			Data from SPI data register (SPDR)
 *	@date 05.12.16			first implementation					*/
static inline uint8_t spiRxByte(void)
{
	return SPDR;
}

/*! @brief Wait for SPIF and read byte from SPI
 *
 *	This will automatically clear the SPIF after reading the data
 *  from SPDR (see datasheet p. 223, 23.5.2 SPI Status Register 0). 
 *
 *	@return uint8_t			Data from SPI data register (SPDR)
 *	@date 05.12.16			first implementation					*/
static inline uint8_t spiWaitRxByte(void)
{
	// Poll SPIF until set
	while(!spiGetIF()){;}

	return spiRxByte();
}

static inline uint8_t spiTransfer(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF))){;}
	return SPDR;
}
#endif /* SPI_COMMON_H_ */