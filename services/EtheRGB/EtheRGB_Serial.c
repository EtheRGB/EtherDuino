/*!	@brief EtheRGB Serial communications module
 *
 *	@author	inselc
 *	@date	21.05.17		First implementation
 *	@date	08.07.17		Reworked Comms module
 *	@date	11.07.17		Restructuring							*/

/*	@todo	Response packets */

#include <stdio.h>
#include <stdint.h>
#include "../../core/Serial/Serial.h"
#include "../../core/Log/Log.h"
#include "EtheRGB_Command.h"
#include "EtheRGB_Serial.h"

/*!	@enum etheRgbSerialState_t
 *	@brief Serial module state machine
 *
 *	Internal state machine used by the serial module
 *
 *	@date 11.07.17			Rework									*/
typedef enum {
	STATE_IDLE,
	STATE_GOT_START_BYTE,
	STATE_GOT_COMMAND_BYTE_READ_DATA,
	STATE_GOT_DATA,
	STATE_GOT_CHECKSUM
} etheRgbSerialState_t;

static uint8_t LocalCommandDataBuffer[ETHERGB_MAX_DATA_LENGTH] = { 0x00 };
static etheRgbCommand_t* SharedCommandBuffer = NULL;
static etheRgbCommand_t LocalCommandBuffer = {ETHERGB_INVALID_COMMAND, LocalCommandDataBuffer, 0, SOURCE_SERIAL};
static etheRgbSerialState_t SerialStateMachine = STATE_IDLE;
static uint16_t TimeoutCounter = 0;

/*!	@brief Initialize the Serial module
 *
 *	@param[in] *commandBuffer	Shared command buffer location
 *	@date 11.07.17			Restructuring							*/
void etheRgbSerial_Init(etheRgbCommand_t* commandBuffer)
{
	SharedCommandBuffer = commandBuffer;
	etheRgbSerial_Reset();

	LOG_MESSAGE(SRC_ETHERGB, "Serial Service initialized.");
}

/*!	@brief Reset the Serial module
 *
 *	Resets the internal state machine into idle state, the local
 *	command buffer to default values and the timeout counter to 0
 *
 *	@date 11.07.17			First implementation					*/ 
void etheRgbSerial_Reset(void)
{
	SerialStateMachine = STATE_IDLE;
	TimeoutCounter = 0;
	LocalCommandBuffer.commandType = ETHERGB_INVALID_COMMAND;
	LocalCommandBuffer.dataLength = 0;
}

/*!	@brief Serial module polling function
 *
 *	This method gets called periodically by the state machine, and
 *	reads any incoming data into the local command buffer.
 *	Incoming data will be checked for protocol/checksum errors on
 *	the fly
 *
 *	@date 11.07.17			Rework									
 *	@return	etheRgbSource	SOURCE_SERIAL, if a complete packet was
 *							received.								*/
etheRgbSource_t etheRgbSerial_Poll(void)
{
	if (SharedCommandBuffer == NULL)
	{
		LOG_CRASH(SRC_ETHERGB, "NULL pointer access at SharedCommandBuffer.");
	}

	if (serialAvailable())
	{
		TimeoutCounter = 0;

		uint8_t data = serialRead();
		switch (SerialStateMachine)
		{
			case STATE_IDLE:
				if (data == ETHERGB_START_BYTE)
				{
					// Advance to next state
					SerialStateMachine = STATE_GOT_START_BYTE;

					// No complete packet, yet
					return SOURCE_NONE;
				}
				else
				{	
					LOG_MESSAGE(SRC_ETHERGB, "Got invalid start byte.");
					etheRgbSerial_Reset();

					return SOURCE_NONE;
				}
			case STATE_GOT_START_BYTE:
				if (etheRgbCommand_HasCommand(data))
				{
					// Copy command to local buffer
					LocalCommandBuffer.commandType = data;

					// Advance to next state
					SerialStateMachine = (etheRgbCommand_GetRequiredDataLength(data) > 0) ? STATE_GOT_COMMAND_BYTE_READ_DATA : STATE_GOT_DATA;

					// No complete packet, yet
					return SOURCE_NONE;
				}
				else
				{
					LOG_MESSAGE(SRC_ETHERGB, "Got invalid command byte.");
					etheRgbSerial_Reset();

					return SOURCE_NONE;
				}
			case STATE_GOT_COMMAND_BYTE_READ_DATA:
				LocalCommandDataBuffer[LocalCommandBuffer.dataLength] = data;
				++LocalCommandBuffer.dataLength;

				if (LocalCommandBuffer.dataLength == etheRgbCommand_GetRequiredDataLength(LocalCommandBuffer.commandType))
				{
					// Data section complete
					SerialStateMachine = STATE_GOT_DATA;
				}

				// No complete packet, yet
				return SOURCE_NONE;
			case STATE_GOT_DATA:
				if (data == etheRgbCommand_CalculateChecksum(&LocalCommandBuffer))
				{
					// Data is complete - copy to shared buffer
					SharedCommandBuffer->commandType = LocalCommandBuffer.commandType;
					for (register uint8_t i=0; i<LocalCommandBuffer.dataLength; ++i)
					{
						SharedCommandBuffer->data[i] = LocalCommandBuffer.data[i];
					}
					SharedCommandBuffer->dataLength = LocalCommandBuffer.dataLength;
					SharedCommandBuffer->source = LocalCommandBuffer.source;

					// Clear old data
					etheRgbSerial_Reset();

					// Got a complete packet
					return SOURCE_SERIAL;
				}
				else
				{
					LOG_MESSAGE(SRC_ETHERGB, "Got invalid checksum.");
					etheRgbSerial_Reset();

					return SOURCE_NONE;
				}
			default:
				LOG_CRASH(SRC_ETHERGB, "Invalid state machine state.");
		}
	}
	else
	{
		++TimeoutCounter;
		if (TimeoutCounter == UINT16_MAX)
		{
			if (SerialStateMachine != STATE_IDLE)
			{
				// Serial connection timed out
				LOG_MESSAGE(SRC_ETHERGB, "Serial connection timed out.");
				etheRgbSerial_Reset();
				return SOURCE_NONE;
			}
		}
	}

	return SOURCE_NONE;
}

/*!	@brief Send response packet via serial connection
 *
 *	@param[in] responseBuffer	Packet buffer to read data from
 *	@date 13.07.17			First implementation					*/
void etheRgbSerial_Send(etheRgbCommand_t* responseBuffer)
{
	if (responseBuffer == NULL)
	{	
		LOG_ERROR(SRC_ETHERGB, "Got NULL as responseBuffer ref.");
		return;
	}

	// Calculate checksum
	uint8_t checksum = etheRgbCommand_CalculateChecksum(responseBuffer);

	// Send data
	serialWriteChar(ETHERGB_START_BYTE);
	serialWriteChar(responseBuffer->commandType);
	serialWriteBuf(responseBuffer->data, responseBuffer->dataLength, -1);
	serialWriteChar(checksum);
}