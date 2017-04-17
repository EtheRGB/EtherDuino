/*! @brief I/O definitions
 *
 *	@author	inselc
 *	@date	04.12.16	initial version								*/

#ifndef IO_H_
#define IO_H_

/*! @file */

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

#define LOW		false	/*!< Logic LOW pin state representation		*/
#define HIGH	true	/*!< Logic HIGH pin state representation	*/

/*! @enum pinDir_t 
 *  @brief I/O pin direction option									*/
typedef enum tagPinDir_t
{
	INPUT = 0,
	OUTPUT = 1
} pinDir_t;

/*! @struct	pin_t
 *  @brief I/O pin type
 *  @var pin_t::Port
 *  Port register address of the selected pin (e.g. &PORTD).
 *  @var pin_t::DDR
 *  Direction register address of the selected pin (e.g. &DDRD).
 *  @var pin_t::PINR
 *	PIN Register name of the selected pin (e.g. &PIND).
 *  @var pin_t::Number
 *  Number (0...8) of the pin at the selected port (e.g. PORTD5, or 5).
 *	@var pin_t::Direction
 *	Input/Output configuration
 *	@var pin_t::PullUp
 *	Pull-Up resistor enabled state									*/
typedef struct tagPin_t
{
	volatile uint8_t*	Port;
	volatile uint8_t*	DDR;
	volatile uint8_t*	PINR;
	uint8_t				Number;
	pinDir_t			Direction;
	bool				PullUp;
} pin_t;

/*! @brief Set direction of I/O pin
 * 
 *	@note Will leave pin in safe mode for PullUp configuration
 *
 *  @param[inout] *pin		Selected pin configuration
 *  @param[in] direction	New pin direction (INPUT/OUTPUT)
 *  @date 04.12.16			first implementation					*/
static inline void ioSetPinDirection(pin_t* pin, pinDir_t direction)
{
	// update configuration
	pin->Direction = direction;

	// set up safe transition state  (Tri-State) as per
	// datasheet p. 99 (18.2.3 Switching Between Input and Output
	*pin->DDR &= ~(1 << pin->Number);
	*pin->Port &= ~(1 << pin->Number);

	// apply new configuration
	*pin->DDR &= ~(!direction << pin->Number);
	*pin->DDR |= direction << pin->Number;
}

/*! @brief Read the digital pin state
 *
 *  @param[in] pin			Selected pin
 *	@return bool			Pin state
 *  @date 04.12.16			first implementation					*/
static inline bool ioReadPin(pin_t pin)
{
	// return pin state as boolean
	return (bool)(*pin.Port & (1 << pin.Number));
}

/*! @brief Set the digital pin state
 * 
 *	@param[in] pin			Target pin
 *	@param[in] state		Output state
 *	@date 04.12.16			first implementation					*/
static inline void ioWritePin(pin_t pin, bool state)
{
	// apply new state to pin output
	*pin.Port &= ~(!state << pin.Number);
	*pin.Port |= state << pin.Number;
}

/*! @brief Configure Pull-ups for I/O pin
 *
 *	@warning Pin must be set into safe configuration for transition beforehand!
 * 
 *	@param[inout] pin		Selected pin configuration
 *	@param[in] pullUpEnable	New pullup-disable state
 *	@data 04.12.16			first implementation					*/
static inline void ioSetPinPullup(pin_t* pin, bool pullUpEnable)
{
	// apply pull-up only, if pin direction is an output.
	if (pin->Direction == INPUT)
	{
		// update configuration
		pin->PullUp = pullUpEnable;

		// Apply pull-up configuration as per
		// datasheet p. 99 (18.2.3 Switching Between Input and Output)
		ioWritePin(*pin, pin->PullUp);
	}
}

/*! @brief Apply pin configuration to hardware
 *
 *	@note Outputs will be initialised with a default LOW state.
 *
 *	@param[in] pinConf		Pin configuration
 *	@date 04.12.16			first implementation					*/
static inline void ioInitPin(pin_t pinConf)
{
	// Set pin direction in DDR
	ioSetPinDirection(&pinConf, pinConf.Direction);
	// Set pull-up configuration, if pin is an input
	ioSetPinPullup(&pinConf, pinConf.PullUp);
}

/*! @brief Globally disable pull-up resistors via the MCUCR PUD bit.
 *
 *	@param[in] PUDisable	Disable all pull-up resistors
 *	@date 04.12.16			first implementation					*/
static inline void ioSetGlobalPullUp(bool PUDisable)
{
	MCUCR &= ~(PUDisable << PUD);
	MCUCR |= !PUDisable << PUD;
}

/*! @brief Toggle pin state between HIGH and LOW, if pin is configured as output.
 *
 *  @note Pin must be configured as an output!
 * 
 *	@param[in] pin			Target pin
 *	@date 04.12.16			first implementation					*/
static inline void ioTogglePin(pin_t pin)
{
	//if ( (*pin.DDR & (1 << pin.Number)) && (pin.Direction == OUTPUT) )
	{
		// The PIN register is a W1T-type register as per
		// datasheet p. 99 (18.2.2 Toggling the Pin)
		*pin.PINR = 1 << pin.Number;
	}
}

#endif /* IO_H_ */