/*!	@brief Timer 0 Definitions
 *
 *	@author inselc
 *	@date 28.04.17		First implementation						*/

#ifndef TIMER0_H_
#define TIMER0_H_

/*!	@file */

typedef enum {
	TMR0_CLK_OFF = 0x00,			//!< Clock off
	TMR0_CLK_PRESC_DIV_1 = 0x01,	//!< T = T_io
	TMR0_CLK_PRESC_DIV_8 = 0x02,	//!< T = T_io / 8
	TMR0_CLK_PRESC_DIV_64 = 0x03,	//!< T = T_io / 64
	TMR0_CLK_PRESC_DIV_256 = 0x04,	//!< T = T_io / 256
	TMR0_CLK_PRESC_DIV_1024 = 0x05,	//!< T = T_io / 1024
	TMR0_CLK_EXT_FALLING = 0x06,	//!< Ext. clock on T0, falling edge trigger
	TMR0_CLK_EXT_RISING = 0x07		//!< Ext. clock on T0, rising edge trigger
} timer0ClkSrc_t;

typedef enum {
	TMR0_WG_NORMAL = 0x00,			//!< Normal mode
	TMR0_WG_PCPWM = 0x01,			//!< Phase-correct PWM
	TMR0_WG_CTC = 0x02,				//!< Clear Timer on Compare Match
	TMR0_WG_FASTPWM = 0x03,			//!< Fast PWM
	TMR0_WG_PCPWM_C = 0x05,			//!< Phase-correct PWM with match at OCRA
	TMR0_WG_FASTPWM_C = 0x07		//!< Fast PWM with match at OCRA
} timer0WgMode_t;

/*!	@brief Enable Timer0 power
 *
 *	@date 28.04.17		First implementation						*/
static inline void timer0PowerEnable()
{
	PRR &= ~(1 << PRTIM0);
}

/*!	@brief Disable Timer0 power										
 *
 *	@date 28.04.17		First implementation						*/
static inline void timer0PowerDisable(void)
{
	PRR |= 1 << PRTIM0;
}

/*!	@brief Stop Timer0
 *
 *	@date 28.04.17		First implementation						*/
static inline void timer0Stop(void)
{
	// Clear Clock source configuration bits
	TCCR0B &= ~(0x07);
}

/*! @brief Set Timer0 clock source
 *
 *	@param[in] src		Clock/Prescaler configuration
 *	@date 28.04.17		First implementation						*/
static inline void timer0SetClkSrc(timer0ClkSrc_t src)
{
	// Clear current configuration
	timer0Stop();

	// Apply clock source configuration
	TCCR0B |= src & 0x07;
}

/*!	@brief Set Timer0 Waveform Generator mode
 *
 *	@param[in] mode		Waveform Generator mode
 *	@date 28.04.17		First implementation						*/
static inline void timer0SetWaveGenMode(timer0WgMode_t genMode)
{
	// Clear WGM2:0 bits
	TCCR0A &= ~(0x03);
	TCCR0B &= ~(1 << 3);

	// Apply new configuration
	TCCR0A |= genMode & 0x03;
	TCCR0B |= (genMode & 0x04) << 1;
}

/*!	@brief Set Timer0 Compare Register Value
 *
 *	@param[in] value	Compare match value
 *	@date 28.04.17		First implementation						*/
static inline void timer0SetCompareA(uint_fast8_t value)
{
	OCR0A = value;
}
static inline void timer0SetCompareB(uint_fast8_t value)
{
	OCR0B = value;
}

/*!	@brief Initialise Timer0
 *
 *	@date 29.04.17		Extracted from timer0Start					*/
static inline void timer0Init(void)
{
	// Enable power to the timer module
	timer0PowerEnable();

	// Stop any running timers
	timer0Stop();

	// Clear Timer0 interrupt mask
	TIMSK0 = 0;

	// Clear old Timer0 interrupts
	// TIFR is a w1r Register as per
	// datasheet p. 148 (19.9.8 TC0 Interrupt flag register)
	TIFR0 = 0x03;
}

/*!	@brief Start Timer0
 *
 *	@param[in] clkSrc	Clock source
 *	@param[in] genMode	Waveform Generator mode
 *	@date 28.04.17		First implementation						*/
static inline void timer0Start(timer0ClkSrc_t clkSrc, timer0WgMode_t genMode)
{
	// Configure clock source
	timer0SetClkSrc(clkSrc);
	
	// Configure waveform generator mode
	timer0SetWaveGenMode(genMode);
}

#endif // TIMER0_H_