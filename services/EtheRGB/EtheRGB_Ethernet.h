/*!	@brief EtheRGB Ethernet communications module
 *
 *	@author	inselc
 *	@date 21.05.17			First implementation
 *	@date 08.07.17			Reworked comms module
 *	@date 11.07.17			Restructuring
 *	@date 12.07.17			Restructuring
 *	@date 13.07.17			Added responses							*/

#ifndef ETHERGB_ETHERNET_H_
#define ETHERGB_ETHERNET_H_

/*!	@file */

/*extern*/ enum etheRgbSource_t;

void etheRgbEthernet_Init(etheRgbCommand_t* commmandBuffer, socket_t socket, uint16_t port);
void etheRgbEthernet_Reset(void);
void etheRgbEthernet_Close(void);
etheRgbSource_t etheRgbEthernet_Poll(void);
void etheRgbEthernet_Send(etheRgbCommand_t* responseBuffer);

#endif /* ETHERGB_ETHERNET_H_ */