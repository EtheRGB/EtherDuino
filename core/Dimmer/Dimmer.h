/*!	@brief Dimmer functionality using bit angle modulation, control-
 *	              ling pins directly connected to the MCU.
 *
 *	@author inselc
 *	@date 28.04.17			First implementation					*/

#ifndef DIMMER_H_
#define DIMMER_H_

/*!	@file */

#include <stdint.h>
#include "../../modules/io/io.h"

void dimmerInit(pin_t** outputs, uint8_t* values, int outputCount);

#endif /* DIMMER_H_ */