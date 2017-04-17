/*
 * Ethernet.c
 *
 * Created: 12/13/2016 1:57:27 AM
 *  Author: inselc
 */ 

#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include "Ethernet.h"
#include "../Debug/debug.h"
#include "../../drivers/W5100/W5100.h"

/*! @brief (Re)initialize the W5100 NIC
 *
 *  @date 10.04.17			First implementation					*/
void ethInit(void)
{
	// Reset all W5100 registers
	w51eWrite(W5100_REG_MR, (1 << W5100_MR_RST));

	// Enable socket interrupts 0 through 3
	w51eWrite(W5100_REG_IMR, (1 << W5100_IM_IR0) | (1 << W5100_IM_IR1) | (1 << W5100_IM_IR2) | (1 << W5100_IM_IR3));

	// Set retry time to 200ms
	// default value after reset: 0x07D0

	// Set retry count to 8
	// default value after reset: 0x08

	// Set memory allocation to 2K each
	// default values after reset: 0x55
}

/*! @brief Set MAC and IP Addresses of the W5100 NIC
 *
 *  @param[in] mac[6]		Hardware MAC Address
 *	@param[in] subnet[4]	Subnet mask
 *	@param[in] ip[4]		Local IP Address			
 *	@date 13.12.16			First implementation					*/
void ethSetLocalIP(uint8_t mac[6], uint8_t subnet[4], uint8_t ip[4])
{
	// Set MAC
	w51eWrite(W5100_REG_SHAR0, mac[0]);
	w51eWrite(W5100_REG_SHAR1, mac[1]);
	w51eWrite(W5100_REG_SHAR2, mac[2]);
	w51eWrite(W5100_REG_SHAR3, mac[3]);
	w51eWrite(W5100_REG_SHAR4, mac[4]);
	w51eWrite(W5100_REG_SHAR5, mac[5]);

	// Set Subnet
	w51eWrite(W5100_REG_SUBR0, subnet[0]);
	w51eWrite(W5100_REG_SUBR1, subnet[1]);
	w51eWrite(W5100_REG_SUBR2, subnet[2]);
	w51eWrite(W5100_REG_SUBR3, subnet[3]);

	// Set Source IP
	w51eWrite(W5100_REG_SIPR0, ip[0]);
	w51eWrite(W5100_REG_SIPR1, ip[1]);
	w51eWrite(W5100_REG_SIPR2, ip[2]);
	w51eWrite(W5100_REG_SIPR3, ip[3]);
}

/*! @brief Open socket
 *
 *  @param[in] socket		Target socket
 *	@param[in] port			Port number
 *	@param[in] protocol		Socket protocol
 *	@param[in] modeFlags	Additional mode flags
 *	@return uint8_t			0 if successful
 *	@date 10.04.17			First implementation					*/
uint8_t ethSockOpen(socket_t socket, uint16_t port, uint8_t protocol, uint8_t modeFlags)
{
	// Make sure socket is closed, first
	if (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) != W5100_Sn_SR_SOCK_CLOSED)
	{
		ethSockClose(socket);
	}

	// Apply protocol and mode flags
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_MR), (protocol << W5100_Sn_MR_PROTO) | modeFlags);

	// Set source port number
	//w51eWrite(W5100_SRG(socket, W5100_REG_S0_PORT0), (port & 0xFF00) >> 8);
	//w51eWrite(W5100_SRG(socket, W5100_REG_S0_PORT1), (port & 0x00FF));
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_PORT0), port);

	// Open socket
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_OPEN);

	// Block until open
	uint_fast16_t timeoutCounter = 0;
	while ((timeoutCounter < UINT_FAST16_MAX) && (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) == W5100_Sn_SR_SOCK_CLOSED))
	{
		++timeoutCounter;
	}

	// Check if opening timed out or if connection was established
	if (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) == W5100_Sn_SR_SOCK_CLOSED)
	{	
		ERROR_LOG("OpenSocket timeout");
		ethSockClose(socket);
		return 2;
	}

	// Socket is now open
	return 0;
}

/*! @brief Close socket
 *
 *	@param[in] socket		Target socket
 *	@date 10.04.17			First implementation					*/
void ethSockClose(socket_t socket)
{
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_CLOSE);
}

/*! @brief Set up socket into listening state (Server mode)
 *
 *	@param[in] socket		Target socket
 *	@return uint8_t			0 if successful
 *	@date 10.04.17			First implementation					*/
uint8_t ethSockListen(socket_t socket)
{
	// LISTEN mode can only be entered in TCP mode after init

	// Check if socket is initialized properly
	if (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) != W5100_Sn_SR_SOCK_INIT)
	{
		ERROR_LOG("Sock not in INIT mode");
		return 2;
	}

	 // Set socket mode to listen
	 w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_LISTEN);

	 // Wait until socket is in listen mode;
	 uint_fast16_t timeoutCounter = 0;
	 while ((timeoutCounter < UINT_FAST16_MAX) && (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) != W5100_Sn_SR_SOCK_LISTEN))
	 {
		++timeoutCounter;
	 }

	 // Check if listen timed out
	 if (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) != W5100_Sn_SR_SOCK_LISTEN)
	 {
		ERROR_LOG("Could not set LISTEN mode");
		return 3;
	 }

	 // Socket is now in LISTEN mode
	 return 0;
}

/*! @brief Disconnect socket connection
 *
 *	@param[in] socket		Connected socket
 *	@date 10.04.17			First implementation					*/
void ethSockDisconnect(socket_t socket)
{
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_DISCON);
}

/*! @brief Check for data waiting to be read
 *
 *	@param[in] socket		Target socket
 *  @return uint16_t		Number of bytes to be read
 *	@date 11.04.17			First implementation					*/
uint16_t ethAvailable(socket_t socket)	
{
	return w51eReadW(W5100_REG_S0_RX_RSR0);
	//return ((w51eRead(W5100_SRG(socket, W5100_REG_S0_RX_RSR0)) & 0x00FF) << 8) | w51eRead(W5100_SRG(socket, W5100_REG_S0_RX_RSR1));
}

/*! @brief Read data from socket receive memory
 *
 *  @param[in] socket		Socket to read from
 *	@param[out] *dataBuffer	Target buffer to store data
 *	@param[in] bufSize		Buffer size
 *	@return uint16_t		Number of bytes read
 *	@date 11.04.17			First implementation					*/
uint16_t ethRead(socket_t socket, uint8_t* dataBuffer, uint16_t bufSize)
{
	// Check if data is available for reading
	uint16_t bytesAvailable = ethAvailable(socket);
	if (bytesAvailable == 0)
	{
		// Abort, if no data queued
		return 0;
	}

	// Get read pointer and mask from device 
	uint16_t readStart = w51eReadW(W5100_SRG(socket, W5100_REG_S0_RX_RD0)); //((w51eRead(W5100_SRG(socket, W5100_REG_S0_RX_RD0)) & 0x00FF) << 8) | w51eRead(W5100_SRG(socket, W5100_REG_S0_RX_RD1));
	uint16_t mask = (0x400 << ((w51eRead(W5100_REG_RMSR) & (0x3 << (socket * 2))) >> (socket * 2))) - 1;

	// Cyclic read single byte until all data is processed or buffer limit is reached
	uint_fast16_t dataCounter = 0;
	while ((dataCounter < bufSize) && (dataCounter < bytesAvailable))
	{
		//dataBuffer[dataCounter] = w51eRead(W5100_CHIP_BASE + W5100_RXM_BASE + (((readStart + dataCounter) % (mask + 1)) & mask));
		dataBuffer[dataCounter] = w51eRead(W5100_CHIP_BASE + W5100_RXM_BASE + (readStart & mask));
		++readStart;
		++dataCounter;
	}

	// Set up read pointer for next receive operation
	/*w51eWrite(W5100_SRG(socket, W5100_REG_S0_RX_RD0), (readPtr&0xFF00) >> 8);
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_RX_RD1), readPtr&0x00FF);*/
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_RX_RD0), readStart);

	// Incoming data will now be stored starting at the RXD address
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_RECV);

	return dataCounter;
}

// ethReadFrom

/*! @brief Write data to socket memory for transmission
 *
 *	@param[in] socket		Target socket
 *	@param[in] *dataBuffer	Buffer to read data from
 *	@param[in] dataLenght	Number of bytes to be sent
 *	@return uint16_t		Number of bytes actually written
 *	@date 14.04.17			First implementation					*/
uint16_t ethWrite(socket_t socket, uint8_t* dataBuffer, uint16_t dataLength)
{
	// Abort if no data to be written
	if (dataLength == 0)
	{
		return 0;
	}

	// Abort if device transmit memory is full
	uint16_t freeSize = w51eReadW(W5100_SRG(socket, W5100_REG_S0_TX_FSR0));
	if (freeSize == 0)
	{
		return 0;
	}

	// Get send pointer and mask from device
	uint16_t writeStart = w51eReadW(W5100_SRG(socket, W5100_REG_S0_TX_WR0));
	uint16_t mask = (0x400 << ((w51eRead(W5100_REG_TMSR) & (0x3 << (socket * 2))) >> (socket * 2))) - 1;

	// Cyclic write single byte until all data is processed or buffer limit is reached
	uint_fast16_t dataCounter = 0;
	while ((dataCounter < dataLength) && (dataCounter < freeSize))
	{
		//w51eWrite(W5100_CHIP_BASE + W5100_TXM_BASE + (((writeStart + dataCounter) % (mask + 1)) & mask), dataBuffer[dataCounter]);
		w51eWrite(W5100_CHIP_BASE + W5100_TXM_BASE + ((writeStart + dataCounter) & mask), dataBuffer[dataCounter]);
		++dataCounter;
	}

	// Set up write pointer for next transmit operation
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_TX_WR0), (writeStart + dataCounter));

	// Trigger sending data till new TXR address
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_SEND);

	return dataCounter;
}