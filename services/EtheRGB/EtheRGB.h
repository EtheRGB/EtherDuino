/*!	@brief Main EtheRGB module
 *
 *	Setup and control functions for all EtheRGB submodules
 *
 *	@author	inselc
 *	@date 21.05.17			First implementation
 *	@date 25.06.17			Added command module
 *	@date 08.07.17			Reworked comms module
 *	@date 11.07.17			Restructuring							*/

#ifndef ETHERGB_H_
#define ETHERGB_H_

/*!	@file */

void etheRgbInit(socket_t socket, uint16_t port);
void etheRgbPoll(void);

#endif /* ETHERGB_H_ */