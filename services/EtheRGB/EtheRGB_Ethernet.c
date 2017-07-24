/*!	@brief EtheRGB Ethernet communications module
 *
 *	@author	inselc
 *	@date 21.05.17			First implementation
 *	@date 08.07.17			Reworked comms module
 *	@date 12.07.17			Restructuring
 *	@date 13.07.17			Added responses							*/
 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../../core/Ethernet/Ethernet.h"
#include "../../core/Serial/Serial.h"
#include "../../core/Log/Log.h"
#include "EtheRGB_Command.h"
#include "EtheRGB_Ethernet.h"

static socket_t ServerSocket = 0;
static uint16_t ServerPort = 0;
static uint8_t LocalCommandDataBuffer[ETHERGB_MAX_DATA_LENGTH] = { 0x00 };
static etheRgbCommand_t* SharedCommandBuffer = NULL;
static etheRgbCommand_t LocalCommandBuffer = {ETHERGB_INVALID_COMMAND, LocalCommandDataBuffer, 0, SOURCE_ETHERNET};
static uint16_t TimeoutCounter = 0;

/*!	@brief Initialise the Ethernet Module
 *
 *	@param[in] commandBuffer	Shared command buffer	
 *	@param[in] socket		Server socket
 *	@param[in] port			Server port								
 *	@date 12.07.17			Rework									*/
void etheRgbEthernet_Init(etheRgbCommand_t* commmandBuffer, socket_t socket, uint16_t port)
{
	SharedCommandBuffer = commmandBuffer;
	ServerSocket = socket;
	ServerPort = port;
	etheRgbEthernet_Reset();

	// Force reset on next poll
	ethSockClose(ServerSocket);

	LOG_MESSAGE(SRC_ETHERGB, "Ethernet Service initialized.");
}

/*!	@brief Reset the Ethernet module 
 *
 *	Resets the timeout counter and the local command buffer
 *
 *	@date 12.07.17			Rework									*/
void etheRgbEthernet_Reset(void)
{
	TimeoutCounter = 0;
	LocalCommandBuffer.dataLength = 0;
	LocalCommandBuffer.commandType = ETHERGB_INVALID_COMMAND;
	LocalCommandBuffer.source = SOURCE_ETHERNET;
}

/*!	@brief Close the Ethernet socket
 *
 *	Closes the ethernet socket used by the ethernet module
 *
 *	@date 12.07.17			Rework									*/
void etheRgbEthernet_Close(void)
{
	ethSockClose(ServerSocket);

	LOG_MESSAGE(SRC_ETHERGB, "Ethernet Socket closed.");
}

/*!	@brief Ethernet Moudule Polling Function
 *
 *	This function is called periodically by the EtheRGB state 
 *	machine.
 *
 *	@return etheRgbSource_t	SOURCE_ETHERNET, if packet complete
 *	@date 12.07.17			Rework									*/
etheRgbSource_t etheRgbEthernet_Poll(void)
{
	if (SharedCommandBuffer == NULL)
	{
		LOG_CRASH(SRC_ETHERGB, "NULL pointer access at SharedCommandBuffer.");
	}

	if (ethIsClosed(ServerSocket))
	{
		LOG_MESSAGE(SRC_ETHERGB, "Socket closed. Reopening...");
		if (!ethSockOpen(ServerSocket, ServerPort, W5100_Sn_MR_PROTO_TCP, 0))
		{
			LOG_CRASH(SRC_ETHERGB, "Could not open server socket.");
		}
		if (!ethSockListen(ServerSocket))
		{
			LOG_CRASH(SRC_ETHERGB, "Could not start listening on server socket.")
		}
	}
	else if(ethIsClosing(ServerSocket))
	{
		LOG_MESSAGE(SRC_ETHERGB, "Socket closing. Disconnecting...");
		ethSockDisconnect(ServerSocket);
	}

	if (ethAvailable(ServerSocket))
	{
		TimeoutCounter = 0;

		uint8_t data[ETHERGB_MAX_DATA_LENGTH + 3];
		uint8_t dataLength = ethRead(ServerSocket, data, ETHERGB_MAX_DATA_LENGTH + 3);

		if (dataLength < 3)
		{
			LOG_MESSAGE(SRC_ETHERGB, "Message too short.");
			etheRgbEthernet_Close();
			etheRgbEthernet_Reset();

			return SOURCE_NONE;
		}
		else
		{
			// Check start byte
			if (data[0] != ETHERGB_START_BYTE)
			{
				LOG_MESSAGE(SRC_ETHERGB, "Got invalid start byte.");
				etheRgbEthernet_Close();
				etheRgbEthernet_Reset();

				// No complete packet
				return SOURCE_NONE;
			}

			// Check command byte
			if (!etheRgbCommand_HasCommand(data[1]))
			{
				LOG_MESSAGE(SRC_ETHERGB, "Got invalid command byte.");
				etheRgbEthernet_Close();
				etheRgbEthernet_Reset();

				// No complete packet
				return SOURCE_NONE;
			}
			LocalCommandBuffer.commandType = data[1];

			// Check data length
			LocalCommandBuffer.dataLength = dataLength - 3;
			uint8_t expectedDataLength = etheRgbCommand_GetRequiredDataLength(data[1]);
			if (LocalCommandBuffer.dataLength > expectedDataLength)
			{
				LOG_MESSAGE(SRC_ETHERGB, "Data too long");
				etheRgbEthernet_Close();
				etheRgbEthernet_Reset();

				// No complete packet
				return SOURCE_NONE;
			}

			// Copy data
			for (int i=0; i < LocalCommandBuffer.dataLength; ++i)
			{	
				LocalCommandBuffer.data[i] = data[i+2];
			}

			// Check checksum
			if (data[2+LocalCommandBuffer.dataLength] != etheRgbCommand_CalculateChecksum(&LocalCommandBuffer))
			{
				LOG_MESSAGE(SRC_ETHERGB, "Got invalid checksum.");
				etheRgbEthernet_Close();
				etheRgbEthernet_Reset();

				// No complete packet
				return SOURCE_NONE;
			}

			// Copy data to shared buffer
			SharedCommandBuffer->commandType = LocalCommandBuffer.commandType;
			for (int i = 0; i < expectedDataLength; ++i)
			{	
				SharedCommandBuffer->data[i] = LocalCommandBuffer.data[i];
			}
			SharedCommandBuffer->dataLength = LocalCommandBuffer.dataLength;
			SharedCommandBuffer->source = LocalCommandBuffer.source;

			// Clear old data, but keep socket open
			etheRgbEthernet_Reset();

			// Packet complete
			return SOURCE_ETHERNET;
		}
	}
	else
	{
		++TimeoutCounter;
		if (TimeoutCounter == UINT16_MAX)
		{	
			if (!ethIsListening(ServerSocket))
			{
				LOG_MESSAGE(SRC_ETHERGB, "Ethernet connection timed out.");
				etheRgbEthernet_Close();
				etheRgbEthernet_Reset();
				return SOURCE_NONE;
			}
		}
	}

	return SOURCE_NONE;
}

/*!	@brief Send response packet via Ethernet
 *
 *	@param[in] responseBuffer	Packet buffer to read data from
 *	@date 13.07.17			First implementation					*/
void etheRgbEthernet_Send(etheRgbCommand_t* responseBuffer)
{
	if (responseBuffer == NULL)
	{
		LOG_ERROR(SRC_ETHERGB, "Got NULL as responseBuffer ref.");
		return;
	}
	
	// Calculate checksum
	uint8_t checksum = etheRgbCommand_CalculateChecksum(responseBuffer);

	// Prepare data
	uint8_t dataBuffer[ETHERGB_MAX_DATA_LENGTH + 3];
	dataBuffer[0] = ETHERGB_START_BYTE;
	dataBuffer[1] = responseBuffer->commandType;
	for (int i=0; i < responseBuffer->dataLength; ++i)
	{	
		dataBuffer[2+i] = responseBuffer->data[i];
	}
	dataBuffer[2+responseBuffer->dataLength] = checksum;

	// Send data packet
	ethWrite(ServerSocket, dataBuffer, responseBuffer->dataLength+3);
}