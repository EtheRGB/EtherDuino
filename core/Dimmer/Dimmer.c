/*!	@brief Dimmer functionality using bit angle modulation, control-
 *	              ling pins directly connected to the MCU.
 *
 *	This dimmer module uses Timer 0 and Output Compare module A 
 *	to modulate the output signals.
 *
 *	@author inselc
 *	@date 28.04.17			First implementation					*/

#include <stdio.h>
#include <avr/interrupt.h>
#include "../../modules/timer/timer.h"
#include "Dimmer.h"

/*!	@brief Precalculated Output Compare Register values for
 *	       bit angle modulation										*/
static const uint8_t dimmerBitAngleTimings[8] = {
	0x01,
	0x03,
	0x07,
	0x0F,
	0x1F,
	0x3F,
	0x7F,
	0xFF
};
volatile static uint8_t dimmerCurrentBit = 0;				//!< Currently displayed bit
static pin_t** dimmerOutputs = NULL;						//!< Array of output pin descriptions
static uint8_t* dimmerValues;								//!< Corresponding channel values
static uint8_t dimmerOutputsCount = 0;						//!< Number of channels

/*!	@brief Initialise the Dimmer module
 *
 *	@param[in] **outputs	Output Pin definitions
 *	@param[in] *values		Channel values
 *	@param[in] outputCount	Number of channels
 *	@date 28.04.17			First implementation					*/
void dimmerInit(pin_t** outputs, uint8_t* values, int outputCount)
{
	// Stop any running timer0
	timer0Stop();
	
	// Setup pointers
	dimmerOutputs = outputs;
	dimmerOutputsCount = outputCount;
	dimmerValues = values;
	dimmerCurrentBit = 0;

	// Initialise Timer 0
	timer0Init();

	// Setup bit timing for Bit 0
	timer0SetCompareA(dimmerBitAngleTimings[0]);

	// Enable Timer 0 Compare A interrupt
	TIMSK0 |= 1 << OCF0A;

	// Start Timer:
	// 256 prescale, Clear timer on OCRA match
	timer0Start(TMR0_CLK_PRESC_DIV_256, TMR0_WG_CTC);
}

// -----------------------------------------------------------------

/*!	@brief Timer 0 Compare Match ISR: bit angle modulation
 *
 *	@date 28.04.17		First implementation						*/
ISR(TIMER0_COMPA_vect)
{
	for (uint_fast8_t i = 0; i < dimmerOutputsCount; ++i)
	{
		ioWritePin(dimmerOutputs[i], dimmerValues[i] & (1 << (dimmerCurrentBit)));
	}
	timer0SetCompareA(dimmerBitAngleTimings[dimmerCurrentBit]);
	
	// Cycle through bits
	++dimmerCurrentBit;
	dimmerCurrentBit %= 8;
}
