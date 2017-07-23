/*!	@brief Core EtheRGB State Machine
 *
 *	Handles command inputs and processing
 *	
 *	@author	inselc
 *	@date 21.05.17			First implementation
 *	@date 25.06.17			Added command module
 *	@date 08.07.17			Reworked comms module
 *	@date 11.07.17			Restructuring
 *	@date 13.07.17			Added responses							*/

#include <stdio.h>
#include <stdint.h>
#include "../../core/Ethernet/Ethernet.h"
#include "../../core/Serial/Serial.h"
#include "../../core/Log/Log.h"
#include "EtheRGB_Command.h"
#include "EtheRGB_Serial.h"
#include "EtheRGB_Ethernet.h"
#include "EtheRGB_IO.h"
#include "EtheRGB_Dimmer.h"
#include "EtheRGB_StateMachine.h"

typedef enum {
	STATE_UNINIT,
	STATE_IDLE,
	STATE_PROC
} etheRgbStateMachineState_t;

static etheRgbCommand_t* SharedCommandBuffer = NULL;
static etheRgbCommand_t* SharedResponseBuffer = NULL;
static etheRgbStateMachineState_t StateMachineState = STATE_UNINIT;
static uint8_t DataInputSelector = 0;

/*!	@brief Initialize the state machine
 *	
 *	@param[in] *commandBuffer	Shared command buffer
 *	@param[in] *responseBuffer	Shared response buffer
 *	@date 21.05.17			First implementation
 *	@date 11.07.17			Reworked
 *	@date 13.07.17			Added responses							*/
void etheRgbStateMachine_Init(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer)
{
	StateMachineState = STATE_IDLE;
	SharedCommandBuffer = commandBuffer;
	SharedResponseBuffer = responseBuffer;
}

/*!	@brief Reset the state machine
 *
 *	@date 21.05.17			First implementation 
 *	@date 11.07.17			Reworked								*/
void etheRgbStateMachine_Reset(void)
{
	DataInputSelector = 0;
	StateMachineState = STATE_UNINIT;
}

/*!	@brief Clear shared command and response buffers
 *
 *	@date 13.07.17			First implementation					*/
void etheRgbStateMachine_ClearBuffers()
{
	SharedCommandBuffer->commandType = ETHERGB_INVALID_COMMAND;
	SharedCommandBuffer->dataLength = 0;
	SharedCommandBuffer->source = SOURCE_NONE;

	SharedResponseBuffer->dataLength = 0;
}

/*!	@brief State machine state: Idle
 *
 *	Device is waiting for incoming data. Input sources are queried
 *	in a round-robin fashion to prevent blocking by a single source
 *
 *	@date 21.05.17			First implementation 
 *	@date 11.07.17			Reworked								*/
void etheRgbStateMachine_IdleState(void)
{
	switch (DataInputSelector)
	{
		case 0:
			etheRgbIO_Poll();
			break;
		case 1:
			etheRgbSerial_Poll();
			break;
		case 2:
			etheRgbEthernet_Poll();
			break;
		default:
			DataInputSelector = 0;
			return;
	}
	++DataInputSelector;

	// Advance to processing state, when a command has been loaded
	if (SharedCommandBuffer->source != SOURCE_NONE)
	{
		StateMachineState = STATE_PROC;
	}
}

/*!	@brief Process received command 
 *
 *	@date 25.06.17			First implementation 
 *	@date 11.07.17			Reworked
 *	@date 13.07.17			Added responses							*/
void etheRgbStateMachine_ProcState(void)
{
	if (etheRgbCommand_Run())
	{
		// Response data to be sent
		switch (SharedCommandBuffer->source)
		{
			case SOURCE_SERIAL:
				etheRgbSerial_Send(SharedResponseBuffer);
				break;
			case SOURCE_ETHERNET:
				etheRgbEthernet_Send(SharedResponseBuffer);
				break;
			default:
				// Can't send response to IO...
				;
		}
	}

	// Reset to idle state
	StateMachineState = STATE_IDLE;
}

/*!	@brief State machine polling routine
 *
 *	@date 21.05.17			First implementation
 *	@date 25.06.17			Added command processing
 *	@date 11.07.17			Reworked								*/
void etheRgbStateMachine_Poll(void)
{
	if (SharedCommandBuffer == NULL)
	{
		LOG_CRASH(SRC_ETHERGB, "Command buffer is NULL");
	}

	switch (StateMachineState)
	{
		case STATE_IDLE:
			etheRgbStateMachine_IdleState();
			break;
		case STATE_PROC:
			etheRgbStateMachine_ProcState();
			etheRgbStateMachine_ClearBuffers();	
			break;
		default:
			LOG_CRASH(SRC_ETHERGB, "Invalid state");
	}
}