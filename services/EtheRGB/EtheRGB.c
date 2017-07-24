/*!	@brief Main EtheRGB module
 *
 *	Setup and control functions for all EtheRGB submodules
 *
 *	@author	inselc
 *	@date 21.05.17			First implementation
 *	@date 25.06.17			Added command module
 *	@date 08.07.17			Reworked comms module
 *	@date 11.07.17			Restructuring							*/

#include <stdio.h>
#include <stdint.h>
#include "../../core/Serial/Serial.h"
#include "../../core/Log/Log.h"
#include "../../core/Ethernet/Ethernet.h"
#include "EtheRGB_Command.h"
#include "EtheRGB_Dimmer.h"
#include "EtheRGB_IO.h"
#include "EtheRGB_Serial.h"
#include "EtheRGB_Ethernet.h"
#include "EtheRGB_StateMachine.h"

static uint8_t SharedCommandDataBuffer[ETHERGB_MAX_DATA_LENGTH] = {0x00};
static etheRgbCommand_t SharedCommandBuffer = {ETHERGB_INVALID_COMMAND, SharedCommandDataBuffer, 0, SOURCE_NONE};
static uint8_t SharedResponseDataBuffer[ETHERGB_MAX_DATA_LENGTH] = {0x00};
static etheRgbCommand_t SharedResponseBuffer = {ETHERGB_INVALID_COMMAND, SharedResponseDataBuffer, 0, SOURCE_NONE};

/*	@brief Initialize the EtheRGB service
 *
 *	@param[in] socket		Server socket
 *	@param[in] port			Server port								*/
void etheRgbInit(socket_t socket, uint16_t port)
{
	etheRgbIO_Init(&SharedCommandBuffer);
	etheRgbSerial_Init(&SharedCommandBuffer);
	etheRgbEthernet_Init(&SharedCommandBuffer, socket, port);
	etheRgbStateMachine_Init(&SharedCommandBuffer, &SharedResponseBuffer);
	etheRgbCommand_Init(&SharedCommandBuffer, &SharedResponseBuffer);
	etheRgbDimmer_Init();
}

void etheRgbPoll(void)
{
	etheRgbStateMachine_Poll();
	etheRgbDimmer_Poll();
}