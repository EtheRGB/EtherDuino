/*!	@brief EtheRGB Command module response packet builders
 *
 *	@author	inselc
 *	@date 13.07.17			First implementation					*/

#include <stdio.h>
#include <stdint.h>
#include "../../core/Serial/Serial.h"
#include "../../core/Log/Log.h"
#include "EtheRGB_Command.h"
#include "EtheRGB_Command_Responses.h"

/*!	@brief Populate buffer with a status response
 *
 *	@param[out] *responseBuffer	Target buffer
 *	@param[in] status		Status code								
 *	@date 13.07.17			First implementation					*/
void etheRgbCommand_SetStatusResponse(etheRgbCommand_t* responseBuffer, uint16_t status)
{
	if (responseBuffer == NULL)
	{	
		LOG_ERROR(SRC_ETHERGB, "Response buffer is NULL");
		return;
	}

	responseBuffer->commandType = 0x01;
	responseBuffer->dataLength = 2;	/* Status = 16 bits */
	responseBuffer->data[0] = (status >> 8) & 0xFF;
	responseBuffer->data[1] = status & 0xFF;
	responseBuffer->data[2] = etheRgbCommand_CalculateChecksum(responseBuffer);
}