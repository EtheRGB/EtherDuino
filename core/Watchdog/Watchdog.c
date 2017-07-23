/*!	@brief System Watchdog Timer module
 *
 *	@author	inselc
 *	@date 15.04.17			First implementation					*/

#include "Watchdog.h"
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../Serial/Serial.h" 
#include "../Log/Log.h"

bool gracefulReset;

/*! @brief Check, whether the system is booting after a reset
 *		   caused by an error.
 *
 *	@return bool			Whether the system was reset due to an
 *							error.
 *	@date 15.04.17			First implementation					*/

/*! @brief Set up the system watchdog timer
 *
 *	@note The watchdog needs to be reset ("petted") every 120ms,
 *		  or the system will be reset with an error
 *
 *	@date 15.04.17			First implementation
 *	@date 23.04.17			Adjusted timeout time					*/
void wdogInit(void)
{
	// Default to error-reset
	gracefulReset = false;
	
	cli();
	wdt_enable(WDTO_2S);

	// Interrupt+System Reset Mode
	WDTCSR |= (1 << WDE) | (1 << WDIE);

	// Reset timer
	wdt_reset();
	sei();
}

/*! @brief Graceful system reboot
 *
 *	Holds execution till the watchdog timer times out
 *
 *	@date 15.04.17			First implementation					*/
void __attribute__((noreturn,naked)) wdogReboot(void)
{
	gracefulReset = true;
	
	while(1)
	{
		;
	}
}

/*! @brief Watchdog timer interrupt service routine
 *
 *	Checks, whether the timeout was intentional, and reboots the
 *	system.
 *
 *	@date 15.04.17			First implementation
 *	@date 12.07.17			Improved logging						*/
ISR(WDT_vect)
{
	cli();
	if (gracefulReset)
	{
		LOG_MESSAGE(SRC_SYSTEM, "Rebooting...\r\n");
		; // Save stuff
	}
	else
	{
		LOG_ERROR(SRC_SYSTEM, "Watchdog timeout. Rebooting...\r\n");
		; // Display error message
	}

	// WDT is now in system-reset mode
	sei();
	while(1)
	{
		// Wait for WDT to trigger reset
		;
	}
}

