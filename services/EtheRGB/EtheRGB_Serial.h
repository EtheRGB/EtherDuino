/*!	@brief EtheRGB Serial communications module
 *
 *	@author	inselc
 *	@date	21.05.17		First implementation
 *	@date	08.07.17		Reworked Comms module
 *	@date	11.07.17		Restructuring					*/

#ifndef ETHERGB_SERIAL_H_
#define ETHERGB_SERIAL_H_

/*!	@file */

/*extern*/ enum etheRgbSource_t;

void etheRgbSerial_Init(etheRgbCommand_t* commandBuffer);
void etheRgbSerial_Reset(void);
etheRgbSource_t etheRgbSerial_Poll(void);
void etheRgbSerial_Send(etheRgbCommand_t* responseBuffer);

#endif /* ETHERGB_SERIAL_H_ */