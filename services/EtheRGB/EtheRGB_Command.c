/*!	@brief EtheRGB Command Module
 *
 *	Handles identifying and processing commands
 *
 *	@author	inselc
 *	@date 08.07.17			First implementation
 *	@date 11.07.17			Restructuring
 *	@date 15.07.17			Added new commands
 *	@date 18.07.17			Added new commands
 *	@date 23.07.17			Added new commands						*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../core/Serial/Serial.h"
#include "../../core/Log/Log.h"
#include "../../tools/ArrayTools.h"
#include "EtheRGB_Command.h"
#include "EtheRGB_Command_Commands.h"
#include "EtheRGB_Command_Responses.h"

// Commands will return true if data needs to be sent back
typedef struct __attribute__((packed)) {
	uint8_t command;
	uint8_t requiredDataLength;
	etheRgbCommandFunc_t function;
} etheRgbCommandMap_t;

static etheRgbCommand_t* SharedCommandBuffer = NULL;
static etheRgbCommand_t* SharedResponseBuffer = NULL;

/*!	@brief Mapping of command type values to functions
 *
 *	@date 08.07.17			First implementation
 *	@date 11.07.17			Restructuring
 *	@date 15.07.17			Added single channel commands
 *	@date 18.07.17			Added GroupColor commands
 *	@date 23.07.17			Added SetIP and Reboot					*/
static const etheRgbCommandMap_t AVAILABLE_COMMANDS[] = {
	{ (uint8_t)'t', 0, Command_Test },
	{ 0x01, 2, Command_SetChannelValue },
	{ 0x02, 3, Command_FadeChannelValue },
	{ 0x03, 4, Command_SetGroupColor },
	{ 0xF0, 4, Command_SetIpAddress },
	{ 0xFE, 0, Command_Reboot }
};

/*!	@brief Initialize the command handler module
 *
 *	@param[in] *commandBuffer	Data source buffer
 *	@param[in] *responseBuffer	Shared response buffer
 *	@date 08.07.17			First implementation
 *	@date 11.07.17			Rework									*/
void etheRgbCommand_Init(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer)
{
	SharedCommandBuffer = commandBuffer;
	SharedResponseBuffer = responseBuffer;
}

/*!	@brief Check, if command list contains command type value
 *
 *	@param[in] commandType	Value to search for
 *	@return	bool			true, if list contains command type		
 *	@date 08.07.17			First implementation
 *	@date 11.07.17			Rework									*/
bool etheRgbCommand_HasCommand(uint8_t commandType)
{
	for (int i=0; i < LENGTHOF(AVAILABLE_COMMANDS); ++i)
	{
		if (AVAILABLE_COMMANDS[i].command == commandType)
		{
			// Found command
			return true;
		}
	}

	// Command has not been found
	return false;
}

/*!	@brief Get required data length for given command type
 *
 *	Required to predict the expected amount of incoming data
 *
 *	@param[in] commandType	Command type to look for
 *	@return uint8_t			Data length for command type, or 0
 *	@date 11.07.17			First implementation					*/
uint8_t etheRgbCommand_GetRequiredDataLength(uint8_t commandType)
{
	for (int i=0; i < LENGTHOF(AVAILABLE_COMMANDS); ++i)
	{
		if (AVAILABLE_COMMANDS[i].command == commandType)
		{
			return AVAILABLE_COMMANDS[i].requiredDataLength;
		}
	}

	return 0;
}

/*!	@brief Calculate even parity for a single data byte
 *
 *	@param[in] data			Source data
 *	@return uint8_t			Even parity								
 *	@date 11.07.17			First implementation					*/
uint8_t etheRgbCommand_CalculateEvenParity(uint8_t data)
{
	uint8_t parity = 0;
	
	for (int i=0; i < 8; ++i)
	{
		if (data & (1 << i))
			parity ^= 0x01;
	}

	return parity;
}

/*!	@brief Calculate the checksum for a command or response packet
 *
 *	@param[in] *command	Source data
 *	@return uint8_t			Checksum
 *	@date 11.07.17			First implementation					*/
uint8_t etheRgbCommand_CalculateChecksum(etheRgbCommand_t* command)
{
	if (command == NULL)
	{	
		LOG_CRASH(SRC_ETHERGB, "Calculate Checksum command is NULL.");
	}

	uint8_t checksum = etheRgbCommand_CalculateEvenParity(ETHERGB_START_BYTE);
	checksum += etheRgbCommand_CalculateEvenParity(command->commandType);
	
	for (int i=0; i < command->dataLength; ++i)
	{	
		checksum += etheRgbCommand_CalculateEvenParity(command->data[i]);
	}

	return checksum;
}

/*!	@brief Command module polling function 
 *
 *	Checks, whether the currently loaded command in the shared
 *	command buffer is executable, and runs it.
 *
 *	@return bool			
 *	@date 08.07.17			First implementation
 *	@date 11.07.17			Rework									*/
bool etheRgbCommand_Run(void)
{
	if (SharedCommandBuffer == NULL || SharedResponseBuffer == NULL)
	{
		LOG_CRASH(SRC_ETHERGB, "Shared Buffer is NULL.");
	}

	for (int i=0; i < LENGTHOF(AVAILABLE_COMMANDS); ++i)
	{
		if (AVAILABLE_COMMANDS[i].command == SharedCommandBuffer->commandType)
		{
			if (AVAILABLE_COMMANDS[i].function == NULL)
			{
				LOG_ERROR(SRC_ETHERGB, "Command function ptr is NULL.");
				return false;
			}
			
			// Run command function
			return AVAILABLE_COMMANDS[i].function(SharedCommandBuffer, SharedResponseBuffer);
		}
	}

	return false;
}
