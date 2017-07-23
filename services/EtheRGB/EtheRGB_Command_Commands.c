/*!	@brief EtheRGB Command module command function 
 *	       implementations
 *
 *	@author	inselc
 *	@date 13.07.17			Moved from EtheRGB_Command
 *	@date 15.07.17			Added Single channel commands
 *	@date 18.07.17			Added GroupColor commands
 *	@date 23.07.17			Added SetIP and Reboot					*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../core/Serial/Serial.h"
#include "../../core/Log/Log.h"
#include "../../core/Watchdog/Watchdog.h"
#include "EtheRGB_Command.h"
#include "EtheRGB_Command_Commands.h"
#include "EtheRGB_Command_Responses.h"
#include "EtheRGB_Dimmer.h"
#include "EtheRGB_Config.h"

/*!	@brief Test command
 *
 *	Will always send a "STATUS OK" response
 *
 *	@return bool			true
 *	@date 13.07.17			First implementation					*/
bool Command_Test(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer)
{
	LOG_MESSAGE(SRC_ETHERGB, "-- Test command received! --");
	etheRgbCommand_SetStatusResponse(responseBuffer, STATUS_OK);
	return true;
}

/*!	@brief Set Channel value
 *
 *	Set a single channel's value without fading
 *
 *	@return bool			true
 *	@date 15.07.17			First implementation					*/
bool Command_SetChannelValue(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer)
{
	etheRgbDimmer_SetChannelValue(commandBuffer->data[0], commandBuffer->data[1]);
	etheRgbCommand_SetStatusResponse(responseBuffer, STATUS_OK);
	return true;
}

/*!	@brief Fade channel value
 *
 *	Fade a single channel's value smoothly to a target value
 *
 *	@return bool			true
 *	@date 15.07.17			First implementation					*/
bool Command_FadeChannelValue(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer)
{
	etheRgbDimmer_SetChannelFadeSpeed(commandBuffer->data[0], commandBuffer->data[2]);
	etheRgbDimmer_SetChannelFadeValue(commandBuffer->data[0], commandBuffer->data[1]);
	etheRgbCommand_SetStatusResponse(responseBuffer, STATUS_OK);
	return true;
}

/*!	@brief Set a group color value
 *
 *	Set a group's color without fading
 *
 *	@return bool			true
 *	@date 18.07.17			First implementation					*/
bool Command_SetGroupColor(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer)
{
	if (commandBuffer->data[0] >= (ETHERGB_MAX_OUTPUT_PINS - 3))
	{
		LOG_ERROR(SRC_ETHERGB, "Group start out of range");
		etheRgbCommand_SetStatusResponse(responseBuffer, STATUS_ERROR);
		return true;
	}

	etheRgbDimmer_SetChannelValue(commandBuffer->data[0], commandBuffer->data[1]);		// R
	etheRgbDimmer_SetChannelValue(commandBuffer->data[0]+1, commandBuffer->data[2]);	// G
	etheRgbDimmer_SetChannelValue(commandBuffer->data[0]+2, commandBuffer->data[3]);	// B
	etheRgbCommand_SetStatusResponse(responseBuffer, STATUS_OK);
	return true;
}

/*!	@brief Save a new IP Address to EEPROM
 *
 *	Writes a new static IP to the device's internal EEPROM.
 *	To limit accidental writes, the IP can only be set once per reboot.
 *
 *	@return bool			true
 *	@date 23.07.17			First implementation					*/
bool Command_SetIpAddress(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer)
{
	static bool writeLimit = false; // Limit writing to one time per reboot

	if (!writeLimit)
	{
		uint8_t currentIP[4];
		eeprom_read_block(currentIP, EtheRgbServerIpAddress, 4);
		if (currentIP[0] != commandBuffer->data[0])
		{
			eeprom_busy_wait();
			writeLimit = true;
			eeprom_write_byte(EtheRgbServerIpAddress, commandBuffer->data[0]);
		}
		if (currentIP[1] != commandBuffer->data[1])
		{
			eeprom_busy_wait();
			writeLimit = true;
			eeprom_write_byte(EtheRgbServerIpAddress+1, commandBuffer->data[1]);
		}
		if (currentIP[2] != commandBuffer->data[2])
		{
			eeprom_busy_wait();
			writeLimit = true;
			eeprom_write_byte(EtheRgbServerIpAddress+2, commandBuffer->data[2]);
		}
		if (currentIP[3] != commandBuffer->data[3])
		{
			eeprom_busy_wait();
			writeLimit = true;
			eeprom_write_byte(EtheRgbServerIpAddress+3, commandBuffer->data[3]);
		}
		
		if (writeLimit)
			etheRgbCommand_SetStatusResponse(responseBuffer, STATUS_OK);
		else
			etheRgbCommand_SetStatusResponse(responseBuffer, STATUS_ERROR);
	}
	else
	{
		etheRgbCommand_SetStatusResponse(responseBuffer, STATUS_ERROR);
	}
	return true;
}

/*!	@brief Reboot the device
 *
 *	Hold until watchdog timeout.
 *	This command will not send a response!
 *
 *	@return bool			false
 *	@date 23.07.17			First implementation					*/
bool Command_Reboot(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer)
{
	wdogReboot();
	return false;
}