/*!	@brief Main program file
 *
 *	@author inselc
 *	@date	01.12.16		First implementation 
 *	@date	...				Various changes, debugging
 *	@date	23.07.17		Version 0.1 R1, Cleanup					*/

/*!	@file */

/*	System clock frequency definition 
 *	The MCU core clock will be runningat 16MHz from an external XT	*/
#define F_CPU 16000000UL

/*	Header includes:
	System headers													*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

/*	Header includes:
	Core, drivers and modules										*/
#include "modules/io/io.h"
#include "modules/spi/spi.h"
#include "core/Serial/Serial.h"
#include "core/Dimmer/Dimmer.h"
#include "drivers/W5100/W5100.h"
#include "core/Ethernet/Ethernet.h"
#include "core/Watchdog/Watchdog.h"
#include "core/Log/Log.h"
#include "services/EtheRGB/EtheRGB.h"
#include "services/EtheRGB/EtheRGB_Dimmer.h"
#include "services/EtheRGB/EtheRGB_Config.h"

/*	Program version definition										*/
#define VERSION_MAJOR	0
#define VERSION_MINOR	1
#define VERSION_REV		1
#define VERSION_STR		"Version " STR(VERSION_MAJOR) "." STR(VERSION_MINOR) " R" STR(VERSION_REV)

/*	Device Network card configuration
 *	Default subnet and MAC address									*/
static uint8_t subnet[4] = {255,255,255,0};
static uint8_t mac[6] = {0x00,0x08,0xDC,0x00,0x00,0x01};

/*!	@brief Program entry point
 *	
 *	@date 01.12.16			First implementation 
 *	@date ...				Various changes
 *	@date 23.07.17			Cleanup									*/
int main(void)
{
	// Make sure the Watchdog is stopped
	wdogStop();

	// Set up UART
	serialInit(USART_BAUD_16M_1M_2X);

	// Startup message, version information
	LOG_MESSAGE(SRC_SYSTEM, "Starting EtherDuino...");
	LOG_MESSAGE(SRC_SYSTEM, VERSION_STR);
	LOG_MESSAGE(SRC_SYSTEM, "Built: " __TIMESTAMP__);  

	// Set up SPI
	LOG_MESSAGE(SRC_SYSTEM, "Initializing SPI Master...");
	spiInitMaster(SPI_FOSC_DIV_2, SPI_MODE0, SPI_MSBFIRST);

	// Initialize the Watchdog, in case NIC setup hangs
	LOG_MESSAGE(SRC_SYSTEM, "Initializing Watchdog Timer...");
	wdogInit();
	sei();

	// Initialize the NIC driver
	LOG_MESSAGE(SRC_SYSTEM, "Initializing NIC...");
	w51eInit();
	
	// Configure the NIC
	LOG_MESSAGE(SRC_SYSTEM, "Reading static IP address from EEPROM...");
	eeprom_busy_wait();
	uint8_t staticIP[4];
	eeprom_read_block(staticIP, EtheRgbServerIpAddress, 4);
	if ((staticIP[0] == 0xFF) && (staticIP[1] == 0xFF) && (staticIP[2] == 0xFF) && (staticIP[3] == 0xFF))
	{
		LOG_MESSAGE(SRC_SYSTEM, "EEPROM uninitialized, using default IP 192.168.178.42");
		staticIP[0] = 192;
		staticIP[1] = 168;
		staticIP[2] = 178;
		staticIP[3] = 42;
	}
	else
	{
		char ipStr[4][4];
		itoa(staticIP[0], ipStr[0], 10);
		itoa(staticIP[1], ipStr[1], 10);
		itoa(staticIP[2], ipStr[2], 10);
		itoa(staticIP[3], ipStr[3], 10);
		serialWriteStr(SRC_ETHERNET);
		serialWriteStr("\tUsing IP address: ");
		serialWriteStr(ipStr[0]);
		serialWriteChar('.');
		serialWriteStr(ipStr[1]);
		serialWriteChar('.');
		serialWriteStr(ipStr[2]);
		serialWriteChar('.');
		serialWriteStr(ipStr[3]);
		serialWriteStr("\r\n");
	}
	
	LOG_MESSAGE(SRC_SYSTEM, "Setting up NIC ...");
 	ethInit((W5100_MSR_1K << W5100_MSR_S0) |	/* Socket 0 TX = 1K */ \
 			(W5100_MSR_2K << W5100_MSR_S1) |	/* Socket 1 TX = 2K */ \
 			(W5100_MSR_2K << W5100_MSR_S2) |	/* Socket 2 TX = 2K */ \
 			(W5100_MSR_4K << W5100_MSR_S3),		/* Socket 3 TX = 4K */ \
 			(W5100_MSR_1K << W5100_MSR_S0) |	/* Socket 0 RX = 1K */ \
 			(W5100_MSR_2K << W5100_MSR_S1) |	/* Socket 1 RX = 2K */ \
 			(W5100_MSR_2K << W5100_MSR_S2) |	/* Socket 2 RX = 2K */ \
 			(W5100_MSR_4K << W5100_MSR_S3));	/* Socket 3 RX = 4K */
 	ethSetLocalIP(mac, subnet, staticIP);
	
	// Start the EtheRGB service on socket 0 port 1234
	LOG_MESSAGE(SRC_SYSTEM, "Initializing EtheRGB service...");
	etheRgbInit(0, 1234);

	// ----------------------- Init done ---------------------------
	LOG_MESSAGE(SRC_SYSTEM, "Ready.");
	
	wdogPet();

	// Main system loop
	while(1)
	{
		wdogPet();
		etheRgbPoll();
	}
}