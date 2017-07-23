/*!	@brief EtheRGB I/O Controller module
 *
 *	Handles button inputs
 *
 *	@author	inselc
 *	@date 10.07.17		First implementation
 *	@date 11.07.17		Restructuring						*/

#ifndef ETHERGB_IO_H_
#define ETHERGB_IO_H_

/*!	@file */

#include "../../modules/io/io.h"

/*extern*/ enum etheRgbSource_t;

/*!	@enum etheRgbIoTrigger_t
 *	@brief Pin trigger mode
 *
 *	@date 11.07.17			Rework; added TRIGGER_CHANGE			*/
typedef enum {
	TRIGGER_NONE,		//!< Never triggers
	TRIGGER_RISING,		//!< Trigger on rising edge
	TRIGGER_FALLING,	//!< Trigger on falling edge
	TRIGGER_CHANGE,		//!< Trigger on rising and falling edges
	TRIGGER_HIGH,		//!< Trigger when signal is HIGH
	TRIGGER_LOW			//!< Trigger when signal is LOW
} etheRgbIoTrigger_t;

/*!	@struct etheRgbPinCommandMapping_t
 *	@brief Mapping of a command to a digital input pin
 *
 *	@date 11.07.17			Rework									*/
typedef struct __attribute__((packed)) {
	pin_t* pin;								//!< Input pin
	etheRgbIoTrigger_t trigger;				//!< Trigger type
	uint8_t command;						//!< Command type
	uint8_t data[ETHERGB_MAX_DATA_LENGTH];	//!< Command data
	uint8_t dataLength;						//!< Command data length
	uint8_t stateHistory;					//!< Previous pin states
} etheRgbPinIoCommandMapping_t;

void etheRgbIO_Init(etheRgbCommand_t* commandBuffer);
void etheRgbIO_Reset(void);
void etheRgbIO_SetupMapping(uint8_t index, pin_t* pin, etheRgbIoTrigger_t trigger, etheRgbCommand_t* command);
void etheRgbIO_GetMapping(uint8_t index, pin_t** pin, etheRgbIoTrigger_t* trigger, etheRgbCommand_t* command);
etheRgbSource_t etheRgbIO_Poll(void);

#endif /* ETHERGB_IO_H_ */