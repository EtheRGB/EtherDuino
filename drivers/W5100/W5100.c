/*! @brief Wiznet W5100 Ethernet Controller driver
 *
 *	@author	inselc
 *	@date	10.12.16		initial version							*/ 

#include "W5100.h"
#include "../../modules/spi/spi.h"
#include "../../modules/io/io.h"

// ChipSelect pin the W5100 is connected to
// Arduino 10 = PB2
static pin_t csPinW5100 = {
	.Port	= &PORTB,
	.DDR = &DDRB,
	.PINR = &PINB,
	.Number = PORTB2,
	.Direction = OUTPUT,
	.PullUp = false
};

/*! @brief Initialise IO for communication with W5100
 *
 *	@date 10.12.16			first implementation					*/
void w51eInit(void)
{
	// Set up GPIO
	ioInitPin(csPinW5100);

	// Default CS disabled
	ioWritePin(csPinW5100, HIGH);
}

/*!	@brief Write data to W5100 register
 *
 *	@param[in] reg			Target register
 *	@param[in] data			Data to write into register
 *	@date 10.12.16			first implementation					*/
void w51eWrite(w51eReg_t reg, uint8_t data)
{
	// Enable ChipSelect
	ioWritePin(csPinW5100, LOW);

	if (spiTransfer(0xF0) != 0x00)	// "Read" opcode
	{
		ioWritePin(csPinW5100, HIGH);
		return;
	}
	if (spiTransfer(reg >> 8) != 0x01)	// Address high byte
	{
		ioWritePin(csPinW5100, HIGH);
		return;
	}
	if(spiTransfer(reg) != 0x02) // Address low byte
	{
		ioWritePin(csPinW5100, HIGH);
		return;
	}
	if(spiTransfer(data) != 0x03)
	{
		ioWritePin(csPinW5100, HIGH);
		return;
	}

	// Disable ChipSelect
	ioWritePin(csPinW5100, HIGH);
}

/*! @brief Read data from W5100 register
 *
 *	@return uint8_t			Data from register
 *	@date 10.12.16			first implementation					*/
uint8_t w51eRead(w51eReg_t reg)
{
	uint8_t c;

	// Enable ChipSelect
	ioWritePin(csPinW5100, LOW);

	// Transmit frame and read data
	if (spiTransfer(0x0F) != 0x00)	// "Read" opcode
	{
		ioWritePin(csPinW5100, HIGH);
		return 0x00;
	}
	if (spiTransfer(reg >> 8) != 0x01)	// Address high byte
	{
		ioWritePin(csPinW5100, HIGH);
		return 0x00;
	}
	if(spiTransfer(reg & 0x00FF) != 0x02) // Address low byte
	{
		ioWritePin(csPinW5100, HIGH);
		return 0x00;
	}
	c = spiTransfer(0xFF);


	// Disable ChipSelect
	ioWritePin(csPinW5100, HIGH);

	return c;
}

