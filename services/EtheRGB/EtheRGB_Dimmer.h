/*!	@brief EtheRGB Dimmer Controller module
 *
 *	@author	inselc
 *	@date 11.07.17			First implementation
 *	@date 14.07.17			Rework
 *	@date 15.07.17			Rework									*/

#ifndef ETHERGB_DIMMER_H_
#define ETHERGB_DIMMER_H_

/*!	@file */
#include "../../modules/io/io.h"

#define ETHERGB_MAX_OUTPUT_PINS 8
#define ETHERGB_MAX_GROUPS 2

void etheRgbDimmer_Init(void);
void etheRgbDimmer_Reset(void);
void etheRgbDimmer_Poll(void);
void etheRgbDimmer_SetChannelValue(uint8_t channel, uint8_t value);
void etheRgbDimmer_SetChannelFadeSpeed(uint8_t channel, uint8_t speed);
void etheRgbDimmer_SetChannelFadeValue(uint8_t channel, uint8_t value);

#endif /* ETHERGB_DIMMER_H_ */