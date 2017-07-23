/*!	@brief EtheRGB I/O Controller module
 *
 *	Handles button inputs
 *
 *	@author	inselc
 *	@date 10.07.17		First implementation
 *	@date 11.07.17		Restructuring								*/

#include <stdio.h>
#include <stdint.h>
#include "../../core/Serial/Serial.h"
#include "../../core/Log/Log.h"
#include "../../modules/io/io.h"
#include "EtheRGB_Command.h"
#include "EtheRGB_IO.h"

#define ETHERGB_MAX_COMMAND_PINS 4
#define ETHERGB_IO_PIN_LAST_STATE 1
#define ETHERGB_IO_PIN_CURRENT_STATE 0

static etheRgbPinIoCommandMapping_t PinCommandMap[ETHERGB_MAX_COMMAND_PINS] = {{NULL, TRIGGER_NONE, ETHERGB_INVALID_COMMAND, {0}, 0, 0}};
static etheRgbCommand_t* SharedCommandBuffer = NULL;
static uint8_t PinCommandMapIndex = 0;

/*!	@brief Initialize the I/O module
 *
 *	@param[in] *commandBuffer	Shared command buffer
 *	@date 11.07.17			Restructuring							*/
void etheRgbIO_Init(etheRgbCommand_t* commandBuffer)
{
	SharedCommandBuffer = commandBuffer;
	etheRgbIO_Reset();
}

/*!	@brief Reset the I/O module
 *
 *	Resets the command map array index to 0
 *
 *	@date 11.07.17			First implementation					*/
void etheRgbIO_Reset(void)
{
	PinCommandMapIndex = 0;
}

/*!	@brief Change a pin-command-mapping entry
 *
 *	@param[in] index		Entry index in mapping array
 *	@param[in] *pin			Input pin
 *	@param[in] trigger		Pin trigger mode
 *	@param[in] *command		Command to be executed when pin triggers
 *	@date 14.07.17			First implementation					*/
void etheRgbIO_SetupMapping(uint8_t index, pin_t* pin, etheRgbIoTrigger_t trigger, etheRgbCommand_t* command)
{
	if (index >= ETHERGB_MAX_COMMAND_PINS)
	{
		LOG_ERROR(SRC_ETHERGB, "Index out of bounds.");
		return;
	}
	if (command == NULL)
	{
		LOG_ERROR(SRC_ETHERGB, "Command arg is NULL.");
		return;
	}

	PinCommandMap[index].pin = pin;
	PinCommandMap[index].trigger = trigger;
	if (command != NULL && command->commandType != ETHERGB_INVALID_COMMAND)
	{
		PinCommandMap[index].command = command->commandType;
		PinCommandMap[index].dataLength = command->dataLength;
		for (int i = 0; i < command->dataLength; ++i)
		{
			PinCommandMap[index].data[i] = command->data[i];
		}
	}
}

/*!	@brief Read a pin-command-mapping entry
 *
 *	@param[in] index		Entry index in mapping array
 *	@param[out] **pin		Input pin
 *	@param[out] *trigger	Pin trigger mode
 *	@param[out]	*command	Command buffer
 *	@date 14.07.17			First implementation					*/
void etheRgbIO_GetMapping(uint8_t index, pin_t** pin, etheRgbIoTrigger_t* trigger, etheRgbCommand_t* command)
{
	if (index > ETHERGB_MAX_COMMAND_PINS)
	{
		LOG_ERROR(SRC_ETHERGB, "Index out of bounds.");
		return;
	}
	if (pin == NULL || *pin == NULL || trigger == NULL || command == NULL)
	{
		LOG_ERROR(SRC_ETHERGB, "Invalid argument.");
	}

	*pin = PinCommandMap[index].pin;
	*trigger = PinCommandMap[index].trigger;
	command->source = SOURCE_IO;
	command->commandType = PinCommandMap[index].command;
	command->dataLength = PinCommandMap[index].dataLength;
	for (int i = 0; i < PinCommandMap[index].dataLength; ++i)
	{
		command->data[i] = PinCommandMap[index].data[i];
	}	
}

/*!	@brief I/O module polling function 
 *
 *	Handles input pin triggering and calls the command associated
 *	with the pin.
 *
 *	@date 11.07.17			Restructuring							*/
etheRgbSource_t etheRgbIO_Poll(void)
{
	if (SharedCommandBuffer == NULL)
	{
		LOG_CRASH(SRC_ETHERGB, "NULL pointer access at SharedCommandBuffer.");
	}

	// Default return value is "no pin triggered"
	etheRgbSource_t returnValue = SOURCE_NONE;

	// Update pin states
	bool currentState = ioReadPin(PinCommandMap[PinCommandMapIndex].pin);
	PinCommandMap[PinCommandMapIndex].stateHistory <<= 1;
	PinCommandMap[PinCommandMapIndex].stateHistory |= currentState << ETHERGB_IO_PIN_CURRENT_STATE;
	bool lastState = PinCommandMap[PinCommandMapIndex].stateHistory & (1 << ETHERGB_IO_PIN_LAST_STATE);

	// Check pin trigger
	if (((PinCommandMap[PinCommandMapIndex].trigger == TRIGGER_RISING) && (currentState == HIGH) && (lastState == LOW)) ||
		((PinCommandMap[PinCommandMapIndex].trigger == TRIGGER_FALLING) && (currentState == LOW) && (lastState == HIGH)) ||
		((PinCommandMap[PinCommandMapIndex].trigger == TRIGGER_CHANGE) && (currentState != lastState)) ||
		((PinCommandMap[PinCommandMapIndex].trigger == TRIGGER_HIGH) && (currentState == HIGH)) ||
		((PinCommandMap[PinCommandMapIndex].trigger == TRIGGER_LOW) && (currentState == LOW)))
	{
		// Pin triggered, copy data to shared command buffer
		SharedCommandBuffer->commandType = PinCommandMap[PinCommandMapIndex].command;
		for (int i=0; i < ETHERGB_MAX_DATA_LENGTH; ++i)
		{
			SharedCommandBuffer->data[i] = PinCommandMap[PinCommandMapIndex].data[i];
		}
		SharedCommandBuffer->dataLength = PinCommandMap[PinCommandMapIndex].dataLength;
		SharedCommandBuffer->source = SOURCE_IO;

		// IO "Packet" complete
		returnValue = SOURCE_IO;
	}

	// Check next pin in next polling cycle
	PinCommandMapIndex = (PinCommandMapIndex + 1) % ETHERGB_MAX_COMMAND_PINS;

	return returnValue;
}