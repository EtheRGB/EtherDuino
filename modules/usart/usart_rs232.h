/*! @brief RS232-specific USART definitions
 *
 *	@author	inselc
 *	@date	05.12.16	initial version								*/ 

#ifndef USART_RS232_H_
#define USART_RS232_H_

#include "usart_common.h"

/*! @file */

/*! @enum usartDataLen_t
 *	Data length configuration										*/
typedef enum tagUsartDataLen_t
{
	USART_LEN_5 = 0x00,
	USART_LEN_6 = 0x01,
	USART_LEN_7 = 0x02,
	USART_LEN_8 = 0x03,
	USART_LEN_9 = 0x07				// Not yet implemented
} usartDataLen_t;

/*! @enum usartParity_t
 *  Parity bit configuration										*/
typedef enum tagUsartParity_t
{
	USART_PARITY_NONE = 0x00,
	USART_PARITY_EVEN = 0x02,
	USART_PARITY_ODD  = 0x03
} usartParity_t;

/*! @enum usartSBit_t 
 *	Stop bit count													*/
typedef enum tagUsartSBit_t
{
	USART_SBIT_1 = 0,
	USART_SBIT_2 = 1
} usartSBit_t;

/*! @brief Set USART character size / data length
 *	
 *	See datasheet p. 250 (Table 24-11 Character Size Settings).
 *
 *  @param[in] dataLen			Data length
 *	@date 05.12.16				first implementation				*/
static inline void usartSetDataLen(usartDataLen_t dataLen)
{
	UCSR0B &= ~((((~dataLen)&0x04) >> 2) << UCSZ02);
	UCSR0C &= ~(((~dataLen)&0x03) << UCSZ00);
	UCSR0B |= ((dataLen&0x04) >> 2) << UCSZ02;
	UCSR0C |= (dataLen&0x03) << UCSZ00;
}

/*! @brief Set USART parity bit generator configuration
 *
 *	See datasheet p. 249 (Table 24-9 USART Mode Selection).
 *
 *	@param[in] parity			Parity bit mode
 *	@date 05.12.16				first implementation				*/
static inline void usartSetParity(usartParity_t parity)
{
	UCSR0C &= ~(((~parity)&0x03) << UPM00);
	UCSR0C |= (parity&0x03) << UPM00;
}

/*! @brief Set USART stop bit configuration
 *
 *	See datasheet p. 250 (Table 24-10 Stop Bit Settings)
 *
 *	@param[in] stopBits			Stop bit configuration
 *	@date 05.12.16				first implementation				*/
static inline void usartSetSBits(usartSBit_t stopBits)
{	
	UCSR0C &= ~(!stopBits << USBS0);
	UCSR0C |= stopBits << USBS0;
}

/*! @brief Setup USART in RS232 mode
 *
 *	@param[in] mode				USART mode (must be R232 sync or R232 async)
 *	@param[in] baudrate			Data baudrate
 *	@param[in] dataLen			Data length per frame (5...9)
 *	@param[in] parity			Parity mode (none, even, odd)
 *  @param[in] stopBits			Stop bits per frame
 *	@date 05.12.16				first implementation				*/
static inline void usartInitRs232(usartMode_t mode,\
							usartBaudrate_t baudrate,\
							usartDataLen_t dataLen,\
							usartParity_t parity,\
							usartSBit_t stopBits)
{
	// Enable USART module power
	usartPowerEnable();

	// force USART disabled for setup
	usartDisable();

	// apply configuration bits
	usartSetMode(mode);
	usartSetBaudrate(baudrate);
	usartSetDataLen(dataLen);
	usartSetParity(parity);
	usartSetSBits(stopBits);

	// re-enable USART
	usartEnable();

	// enable USART interrupts
	UCSR0B |= (0 << TXCIE0) | (1 << RXCIE0);
}
#endif /* USART_RS232_H_ */