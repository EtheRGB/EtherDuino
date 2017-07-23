/*!	@brief EtheRGB Command Module
 *
 *	Handles identifying and processing commands
 *
 *	@author	inselc
 *	@date 08.07.17			First implementation
 *	@date 11.07.17			Restructuring					*/

#ifndef ETHERGB_COMMAND_H_
#define ETHERGB_COMMAND_H_

/*!	@file */

#include <stdint.h>

#define ETHERGB_MAX_DATA_LENGTH	8
#define ETHERGB_START_BYTE (uint8_t)'A'
#define ETHERGB_INVALID_COMMAND (uint8_t)0x00

typedef enum uint8_t {
	SOURCE_NONE,
	SOURCE_SERIAL,
	SOURCE_ETHERNET,
	SOURCE_IO
} etheRgbSource_t;

/*!	@brief Command structure containing command and 
 *	       required data
 *
 *	@date 11.07.17			Rework									*/
typedef struct __attribute__((packed)) {
	uint8_t commandType;	//!< Command number
	uint8_t* data;			//!< Data array
	uint8_t dataLength;		//!< Length of data array
	etheRgbSource_t source;	//!< Source, where the command originated from
} etheRgbCommand_t;

void etheRgbCommand_Init(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer);
bool etheRgbCommand_HasCommand(uint8_t commandType);
uint8_t etheRgbCommand_GetRequiredDataLength(uint8_t commandType);
uint8_t etheRgbCommand_CalculateChecksum(etheRgbCommand_t* command);
bool etheRgbCommand_Run(void);

#endif /* ETHERGB_COMMAND_H_ */