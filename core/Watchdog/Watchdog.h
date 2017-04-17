/*
 * Watchdog.h
 *
 * Created: 4/15/2017 9:48:10 PM
 *  Author: inselc
 */ 


#ifndef WATCHDOG_H_
#define WATCHDOG_H_

/*! @file */

#include <avr/wdt.h>

void wdogInit(void);
void wdogReboot(void);

/*!	@brief Stop any watchdog timer that may be running
 *
 *	@note To be used right after system boot to prevent 
 *		  a reset-loop.
 *
 *	@date 15.04.17			First implementation					*/
static inline __attribute__((__always_inline__)) void wdogStop(void)
{
	wdt_disable();
}

/*! @brief Reset the watchdog timer countdown
 *
 *	@date 15.04.17			First implementation					*/
static inline __attribute__((__always_inline__)) void wdogPet(void)
{
	wdt_reset();
}


#endif /* WATCHDOG_H_ */