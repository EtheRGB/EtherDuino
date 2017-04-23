/*! @brief Serial communication over USART
 *
 *	@author	inselc
 *	@date	05.12.16	initial version								*/ 

#ifndef SERIAL_H_
#define SERIAL_H_

/*! @file */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <avr/pgmspace.h>
#include "../../modules/usart/usart_common.h"
#include "../../modules/usart/usart_rs232.h"

#define SERIAL_MAX_STRLEN	64		/* maximum string length for tx	*/
#define SERIAL_BUF_SIZE		64		/* receive ringbuffer size		*/

/*! @struct serialRingBuffer_t
 *	USART Receive data ring buffer
 *	@var serialRingBuffer_t::head
 *	Ring buffer head index
 *	@var serialRingBuffer_t::tail
 *	Ring buffer tail index
 *	@var serialRingBuffer_t::empty
 *	Ring buffer EMPTY flag
 *	@var serialRingBuffer_t::full
 *	Ring buffer FULL flag
 *  @var serialRingBuffer_t::data
 *	Data array														*/
typedef volatile struct tagSerialRingBuffer_t
{
	uint8_t head;
	uint8_t tail;
	bool empty;
	bool full;
	uint8_t data[SERIAL_BUF_SIZE];
} serialRingBuffer_t;

static serialRingBuffer_t serialRxBuf;

/* Prototypes */
uint8_t serialAvailable(void);
uint8_t serialReadBuf(uint8_t* buffer, uint8_t bufSize, int timeout);
uint8_t serialReadBufUntil(uint8_t* buffer, uint8_t bufSize, char stopChar, int timeout);
uint8_t serialRead(void);
uint8_t serialPeek(void);
int serialWriteStr(const char* message);
int serialWriteStrP(PGM_P message);
uint16_t serialWriteBuf(uint8_t* data, uint16_t length, int timeout);
void serialFlush(void);

/*! @brief Transmit single byte via serial
 *
 *	@param[in] data			Byte to be transmitted
 *	@date 07.12.16			first implementation					*/
static inline void serialWrite(uint8_t data)
{
	usartSendData(data);
}

/*! @brief Quick-Init for Serial over USART in RS232 mode
 *
 *	@note requires interrupts to be enabled.
 *
 *	@param[in] baudrate		USART Baudrate
 *	@date 07.12.16			first implementation					*/
static inline void serialInit(usartBaudrate_t baudrate)
{
	serialFlush();
	usartInitRs232(USART_MSTR_ASYNC, baudrate, USART_LEN_8, USART_PARITY_NONE, USART_SBIT_1);
}
#endif /* SERIAL_H_ */