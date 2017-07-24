/*!	@brief EtheRGB Dimmer Controller module
 *
 *	@author	inselc
 *	@date 11.07.17			First implementation					*/

#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "../../modules/io/io.h"
#include "../../core/Serial/Serial.h"
#include "../../core/Log/Log.h"
#include "../../core/Dimmer/Dimmer.h"
#include "EtheRGB_Dimmer.h"

static pin_t dimPinB = { // R
	.DDR = &DDRD,
	.Direction = OUTPUT,
	.Number = 3,
	.PINR = &PIND,
	.Port = &PORTD,
	.PullUp = false
};
static pin_t dimPinG = { // G
	.DDR = &DDRD,
	.Direction = OUTPUT,
	.Number = 5,
	.PINR = &PIND,
	.Port = &PORTD,
	.PullUp = false
};
static pin_t dimPinR = { // B
	.DDR = &DDRD,
	.Direction = OUTPUT,
	.Number = 6,
	.PINR = &PIND,
	.Port = &PORTD,
	.PullUp = false
};

pin_t* OutputPins[ETHERGB_MAX_OUTPUT_PINS] = { &dimPinR, &dimPinG, &dimPinB };
uint8_t OutputCurrentValues[ETHERGB_MAX_OUTPUT_PINS] = {0x00};
uint8_t OutputFadingCounters[ETHERGB_MAX_OUTPUT_PINS] = {0x00};
uint8_t OutputFadingSpeeds[ETHERGB_MAX_OUTPUT_PINS] = {0x00};
uint8_t OutputTargetValues[ETHERGB_MAX_OUTPUT_PINS] = {0x00};

/*!	@brief Initialize the dimmer module
 *
 *	@todo: Use all 7 output pins
 *
 *	@date 11.07.17			First implementation 
 *	@date 14.07.17			Rework									*/
void etheRgbDimmer_Init(void)
{
	ioInitPin(&dimPinR);
	ioInitPin(&dimPinG);
	ioInitPin(&dimPinB);

	OutputPins[0] = &dimPinR;
	OutputPins[1] = &dimPinG;
	OutputPins[2] = &dimPinB;

	OutputCurrentValues[1] = 0x04;

	dimmerInit(OutputPins, OutputCurrentValues, 3);//ETHERGB_MAX_OUTPUT_PINS);
}

/*!	@brief Reset all dimmer values to initial states (off)
 *
 *	@date 14.07.17			First implementation					*/
void etheRgbDimmer_Reset(void)
{
	for (int i=0; i < ETHERGB_MAX_OUTPUT_PINS; ++i)
	{	
		OutputCurrentValues[i] = 0x00;
		OutputFadingSpeeds[i] = 0x00;
		OutputFadingCounters[i] = 0x00;
		OutputTargetValues[i] = 0x00;
	}
}

/*!	@brief Dimmer module polling routine
 *
 *	Handles channel fading
 *
 *	@date 15.07.17			First implementation					*/
void etheRgbDimmer_Poll(void)
{
	for (int i=0; i < ETHERGB_MAX_OUTPUT_PINS; ++i)
	{	
		if (OutputCurrentValues[i] != OutputTargetValues[i])
		{	
			if (OutputFadingCounters[i] < 0xFF - OutputFadingSpeeds[i])
			{
				OutputFadingCounters[i] += OutputFadingSpeeds[i];
			}
			else
			{
				OutputFadingCounters[i] = 0xFF;
			}
			 
			if (OutputFadingCounters[i] == 0xFF)
			{
				if (OutputCurrentValues[i] < OutputTargetValues[i])
				{
					cli();
					++OutputCurrentValues[i];
					sei();
				}
				else if (OutputCurrentValues[i] > OutputTargetValues[i])
				{
					cli();
					--OutputCurrentValues[i];
					sei();
				}
				
				OutputFadingCounters[i] = 0x00;
			}
		}
	}
}

/*!	@brief Set single channel value
 *
 *	@param[in] channel		Channel number
 *	@param[in] value		Brightness value						
 *	@date 15.07.17			First implementation					*/
void etheRgbDimmer_SetChannelValue(uint8_t channel, uint8_t value)
{
	if (channel >= ETHERGB_MAX_OUTPUT_PINS)
	{
		LOG_ERROR(SRC_ETHERGB, "Index out of bounds.");
		return;
	}

	cli();
	// Pause interrupts to prevent race conditions
	OutputCurrentValues[channel] = value;
	sei();
	OutputTargetValues[channel] = value;
}

/*!	@brief Set a channel's fading speed
 *
 *	@param[in] channel		Channel number
 *	@param[in] speed		Fading speed
 *	@date 15.07.17			First implementation					*/
void etheRgbDimmer_SetChannelFadeSpeed(uint8_t channel, uint8_t speed)
{
	if (channel >= ETHERGB_MAX_OUTPUT_PINS)
	{
		LOG_ERROR(SRC_ETHERGB, "Index out of bounds.");
		return;
	}

	OutputFadingSpeeds[channel] = speed;
}

/*!	@brief Set a channel's fade target value
 *
 *	@param[in] channel		Channel number
 *	@param[in] value		Target brightness value
 *	@date 15.07.17			First implementation					*/
void etheRgbDimmer_SetChannelFadeValue(uint8_t channel, uint8_t value)
{
	if (channel >= ETHERGB_MAX_OUTPUT_PINS)
	{
		LOG_ERROR(SRC_ETHERGB, "Index out of bounds.");
		return;
	}

	OutputTargetValues[channel] = value;
}