/*! @brief Common USART definitions
 *
 *	@author	inselc
 *	@date	05.12.16	initial version								*/ 

#ifndef USART_COMMON_H_
#define USART_COMMON_H_

/*! @file */

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

/*!	@enum usartMode_t
 *	USART Module mode selector.										*/
typedef enum tagUsartMode_t {
	USART_MSTR_ASYNC	= 0x00,
	USART_MSTR_SYNC		= 0x01,
	USART_MSTR_SPI		= 0x40,
	USART_SLAVE_SYNC	= 0x83
} usartMode_t;

/*! @enum usartBaudrate_t
 *	USART Baud rate configuration.									*/
typedef enum tagUsartBaudrate_t {
	USART_BAUD_16M_2400		= 416,			// -0.1%
	USART_BAUD_16M_2400_2X	= 832 | 0x1000,	//  0.0%
	USART_BAUD_16M_4800		= 207,			//  0.2%
	USART_BAUD_16M_4800_2X	= 416 | 0x1000,	// -0.1%
	USART_BAUD_16M_9600		= 103,			//  0.2%
	USART_BAUD_16M_9600_2X	= 207 | 0x1000,	//  0.2%
	USART_BAUD_16M_14400	= 68,			//  0.6%
	USART_BAUD_16M_14400_2X	= 138 | 0x1000,	// -0.1%
	USART_BAUD_16M_19200	= 51,			//  0.2%
	USART_BAUD_16M_19200_2X	= 103 | 0x1000,	//  0.2%
	USART_BAUD_16M_28800	= 34,			// -0.8%
	USART_BAUD_16M_28800_2X	= 68 | 0x1000,	//  0.6%
	USART_BAUD_16M_38400	= 25,			//  0.2%
	USART_BAUD_16M_38400_2X	= 51 | 0x1000,	//  0.2%
	USART_BAUD_16M_57600	= 16,			//  2.1%
	USART_BAUD_16M_57600_2X	= 34 | 0x1000,	// -0.8%
	USART_BAUD_16M_76800	= 12,			//  0.2%
	USART_BAUD_16M_76800_2X	= 25 | 0x1000,	//  0.2%
	USART_BAUD_16M_115200	= 8,			// -3.5%
	USART_BAUD_16M_115200_2X= 16 | 0x1000,	//  2.1%
	USART_BAUD_16M_230400	= 3,			//  8.5%
	USART_BAUD_16M_230400_2X= 8 | 0x1000,	// -3.5%
	USART_BAUD_16M_250000	= 0,			//  0.0%
	USART_BAUD_16M_250000_2X= 7 | 0x1000,	//  0.0%
	USART_BAUD_16M_500000	= 1,			//  0.0%
	USART_BAUD_16M_500000_2X= 3 | 0x1000,	//  0.0%
	USART_BAUD_16M_1M		= 0,			//  0.0%
	USART_BAUD_16M_1M_2X	= 1 | 0x1000,	//  0.0%
	USART_BAUD_16M_2M_2X	= 0 | 0x1000	//	0.0%
} usartBaudrate_t;

/*! @brief Enable power to the USART module
 *
 *	See datasheet p. 225 (24.2 Overview).
 *
 *	@date 05.12.16				first implementation				*/
static inline void usartPowerEnable(void)
{
	PRR &= ~(1 << PRUSART0);
}

/*! @brief Disable power to the USART module
 *
 *	See datasheet p. 225 (24.2 Overview).
 *
 *	@date 05.12.16				first implementation				*/
static inline void usartPowerDisable(void)
{
	PRR |= 1 << PRUSART0;
}

/*! @brief Set USART module mode
 *
 *	See datasheet p. 249 (24.12.4 USART Control and Status Register 0 C).
 *
 *	@param[in] mode				USART mode
 *	@date 05.12.16				first implementation				*/
static inline void usartSetMode(usartMode_t mode)
{
	UCSR0C &= ~(~(mode&0x03) << UPM00);
	UCSR0C |= (mode&0x03) << UPM00;
}

/*! @brief Set USART baud rate
 *
 *	See datasheet p. 243 (Table 24-7 Examples of UBRRn Settings for
 *	Commonly Used Oscillator Frequencies).
 *
 *	@param[in] baudrate			USART baud rate (from enum)
 *	@date 05.12.16				first implementation				*/
static inline void usartSetBaudrate(usartBaudrate_t baudrate)
{
	UBRR0 = baudrate & 0x0FFF;

	UCSR0A &= ~((((~baudrate)&0x1000) >> 12) << U2X0);
	UCSR0A |= ((baudrate&0x1000) >> 12) << U2X0;
}

/*! @brief Wait until the USART data buffer is empty
 *
 *	@date 05.12.16				first implementation				*/
static inline void usartWaitDREmpty(void)
{
	while( !(UCSR0A & (1 << UDRE0)) ){;}
}

/*! @brief Enable USART RTX function
 *
 *	@date 05.12.16				first implementation				*/
static inline void usartEnable(void)
{
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
}

/*! @brief Disable USART RTX function
 *
 *	@date 05.12.16				first implementation				*/
static inline void usartDisable(void)
{
	UCSR0B &= ~((1 << TXEN0) | (1 << RXEN0));
}

/*! @brief Send a single USART data frame
 *
 *	@param[in] data				Data to be transmitted
 *	@date 05.12.16				first implementation				*/
static inline void usartSendData(uint16_t data)
{
	//usartWaitDREmpty();
	UCSR0B &= ~(1 << TXB80);
	UCSR0B |= ((data&0x0100) >> 8) << TXB80;
	UDR0 = data;
}

/*! @brief Read up to 9 data bits from USART RX buffer
 *
 *  @return uint16_t			Data from RX buffer
 *	@date 06.12.16				first implementation				*/
static inline uint16_t usartReadData9(void)
{
	return (UDR0) | (((UCSR0B & (1 << TXB80)) >> TXB80) << 8);
}

/*! @brief Only read up to 8 data bits from USART RX buffer
 *
 *	@return uint8_t				Data from RX buffer
 *	@date 06.12.16				first implementation				*/
static inline uint8_t usartReadData(void)
{
	return UDR0;
}

/*! @brief Check if USART Transmit op is complete
 *
 *	@return bool				TXC bit state
 *	@date 07.12.16				first implementation				*/
static inline bool usartTXC(void)
{
	return (UCSR0A & (1 << TXC0));
}

/*! @brief Check if USART Receive op is complete
 *
 *	@return bool				RXC bit state
 *	@date 07.12.16				first implementation				*/
static inline bool usartRXC(void)
{
	return (UCSR0A & (1 << RXC0));
}

/*! @brief Check if USART data register is empty
 *
 *	@return bool				UDRE bit state
 *	@date 07.12.16				first implementation				*/
static inline bool usartUDRE(void)
{
	return (UCSR0A & (1 << UDRE0));
}

#endif /* USART_COMMON_H_ */