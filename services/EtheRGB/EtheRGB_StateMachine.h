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

#ifndef ETHERGB_STATEMACHINE_H_
#define ETHERGB_STATEMACHINE_H_

/*!	@file */

void etheRgbStateMachine_Init(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer);
void etheRgbStateMachine_Reset(void);
void etheRgbStateMachine_Poll(void);

#endif /* ETHERGB_STATEMACHINE_H_ */