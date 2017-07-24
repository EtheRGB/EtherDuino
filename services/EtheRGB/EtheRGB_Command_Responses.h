/*!	@brief EtheRGB Command module response packet builders
 *
 *	@author	inselc
 *	@date 13.07.17			First implementation					*/

#ifndef ETHERGB_COMMAND_RESPONSES_H_
#define ETHERGB_COMMAND_RESPONSES_H_

#include <stdio.h>
#include <stdint.h>

typedef enum {
	STATUS_OK = 0x0000,
	STATUS_ERROR = 0x0001
} etheRgbStatusCode_t;
void etheRgbCommand_SetStatusResponse(etheRgbCommand_t* responseBuffer, uint16_t status);

#endif /* ETHERGB_COMMAND_RESPONSES_H_ */