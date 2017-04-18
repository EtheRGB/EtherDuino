/*! @brief Serial communication over USART
 *
 *	@author	inselc
 *	@date	05.12.16	initial version								*/ 

/*! @file */

#include <avr/interrupt.h>
#include "Serial.h"

/*! @brief Push data into receive ring buffer
 *
 *	@param[in] data			Data to be pushed into the buffer
 *	@return bool			false, if write failed (buffer full)
 *	@date 07.12.16			first implementation					*/
static bool serialPushRxBuf(uint8_t data)
{
	// Check buffer full
	if (serialRxBuf.full)
	{
		return false;
	}

	// Clear buffer empty flag
	serialRxBuf.empty = false;
	
	// Write data into buffer
	serialRxBuf.data[serialRxBuf.head] = data;
	
	// Advance head pointer
	serialRxBuf.head = (serialRxBuf.head + 1) % SERIAL_BUF_SIZE;

	// Recalculate buffer full flag
	if (serialRxBuf.head == serialRxBuf.tail)
	{
		serialRxBuf.full = true;
	}

	return true;
}

/*! @brief Pop data from receive ring buffer
 *
 *	@return uint8_t			Data at buffer tail. 0x00, if buffer empty
 *	@date 07.12.16			first implementation					*/
static uint8_t serialPopRxBuf(void)
{
	uint8_t data;
	if (serialRxBuf.empty)
	{
		return 0x00;
	}
	else
	{
		// Clear buffer full flag
		serialRxBuf.full = false;

		// Read data from buffer
		data = serialRxBuf.data[serialRxBuf.tail];

		// Advance tail pointer
		serialRxBuf.tail = (serialRxBuf.tail + 1) % SERIAL_BUF_SIZE;

		// Check buffer empty
		if (serialRxBuf.tail == serialRxBuf.head)
		{
			serialRxBuf.empty = true;
		}

		return data;
	}
}

/*! @brief Read from serial into buffer, until ring buffer is empty
 *
 *	@param[out]	*buffer		Target buffer
 *	@param[in] bufSize		Maximum buffer size
 *	@param[in] timeout		Cycles until timeout (-1: infinite)
 *	@return uint8_t			Number of bytes read
 *	@date 07.12.16			first implementation					*/
uint8_t serialReadBuf(uint8_t* buffer, uint8_t bufSize, int timeout)
{
	uint8_t byteCount = 0;
	int timeoutCount = 0;

	// catch Null-pointer exception 
	if (buffer == NULL || bufSize == 0)
		return 0x00;

	// read until ring buffer is empty or target buffer is full
	while (byteCount < bufSize && (timeoutCount < timeout || timeout < 0))
	{
		if (serialRxBuf.empty)
		{
			// Buffer is empty, wait for timeout
			timeoutCount = (timeoutCount + 1) % INT_MAX;
		}
		else
		{
			// Buffer is not empty. Read data
			buffer[byteCount] = serialPopRxBuf();
			byteCount++;

			// Reset timeout counter
			timeoutCount = 0;
		}
	}

	return byteCount;
}

/*! @brief Read from serial into buffer, until character found
 *
 * Will read until char is found or waiting on buffer empty times out,
 * or target buffer is full.
 *
 *	@param[out] *buffer		Target buffer
 *	@param[in] bufSize		Maximum buffer size
 *	@param[in] stopChar		Key char to stop read
 *	@param[in] timeout		Cycles until timeout (-1: infinite)
 *	@return uint8_t			Number of bytes read
 *	@date 07.12.16			first implementation					*/
uint8_t serialReadBufUntil(uint8_t* buffer, uint8_t bufSize, char stopChar, int timeout)
{
	uint8_t byteCounter = 0;
	int timeoutCount = 0;
	
	// Null pointer exception 
	if (buffer == NULL || bufSize == 0)
		return 0x00;

	while(byteCounter < bufSize && (timeoutCount < timeout || timeout < 0))
	{
		if (serialRxBuf.empty)
		{
				timeoutCount = (timeoutCount + 1) % INT_MAX;
		}
		else
		{
			buffer[byteCounter] = serialPopRxBuf();
			timeoutCount = 0;
			byteCounter++;
			
			if (buffer[byteCounter-1] == (uint8_t)stopChar)
			{
				// success, found stop char
				return byteCounter;
			}
		}
	}

	// fail: buffer overflow or timeout
	return byteCounter;
}

/*! @brief Get next byte from receive ring buffer, without removing it
 *
 *	@return uint8_t			Next byte in ring buffer (from tail). 0x00, if empty.
 *	@date 07.12.16			first implementation					*/
uint8_t serialPeek(void)
{
	if (!serialRxBuf.empty)
	{
		return serialRxBuf.data[serialRxBuf.tail];
	}
	else
	{
		return 0x00;
	}
}

/*! @brief Get number of available bytes in ring buffer
 *
 *  @return uint8_t			Number of unread bytes
 *	@date 07.12.16			first implementation					*/
uint8_t serialAvailable(void)
{
	return ((serialRxBuf.head + SERIAL_BUF_SIZE - serialRxBuf.tail) % SERIAL_BUF_SIZE);
}

/*! @brief Read single byte from RX ring buffer
 *
 *	@return	uint8_t			Byte from ring buffer, 0x00 if empty
 *	@date 07.12.16			first implementation					*/
uint8_t serialRead(void)
{
	return serialPopRxBuf();
}

/*! @brief Output string via serial connection (USART RS232)
 *
 * This method will only work, if USART is configured for a
 * character length of 8+ bits!
 *
 *	@param[in] *message			String to be transmitted (\0-term.!)
 *	@return int					Number of characters sent
 *	@date 05.12.16				first implementation				*/
int serialWriteStr(const char* message)
{
	int stringPos = 0;

	// Catch null-pointer exception
	if (message == NULL)
	{
		return -1;
	}

	// Transmit message, including trailing \0
	// Abort, if maximum string length exceeded
	while ((message[stringPos] != '\0') && (stringPos < SERIAL_MAX_STRLEN))
	{
		usartWaitDREmpty();
		usartSendData(message[stringPos]);
		stringPos++;
	}

	return stringPos;
}

/*! @brief Output via serial from non-progmem memory buffer
 *
 * This method will ignore the buffer content, and output until
 * configured byte count is transmitted.
 *
 *	@param[in] *data			Source buffer to transmit from
 *	@param[in] count			Number of bytes to transmit
 *	@param[in] timeout			Cycles until write times out (-1=inf)
 *	@return uint16_t			Number of bytes actually transmitted
 *	@date 07.12.16				first implementation				*/
uint16_t serialWriteBuf(uint8_t* data, uint16_t count, int timeout)
{
	int byteCounter = 0;
	int timeoutCount = 0;

	// Null-pointer exception
	if (data == NULL)
		return 0;

	while (byteCounter < count && (timeoutCount < timeout || timeout < 0))
	{
		if (usartUDRE())
		{
			usartSendData(data[byteCounter]);
			byteCounter++;
		}
		else
		{
			timeoutCount = (timeoutCount + 1) % INT_MAX;
		}
	}

	return byteCounter;
}

/*! @brief Discard remaining data in receive ring buffer
 *
 *	@date 07.12.16				first implementation				*/
void serialFlush(void)
{
	serialRxBuf.head = 0;
	serialRxBuf.tail = 0;
	serialRxBuf.empty = true;
	serialRxBuf.full = false;
}

// -----------------------------------------------------------------

/*! @brief USART Receive ISR: read data into ring buffer
 *
 *	@date 07.12.16				first implementation				*/
ISR(USART_RX_vect)
{
	// copy data from USART buffer into ring buffer
	// this read from UDR will automatically clear the RXC flag
	serialPushRxBuf(usartReadData());
}