/*! @brief Data exchange over Ethernet
 *
 *	@author inselc
 *	@date 13.12.16			Initial version
 *	@date 10.04.17			Added socket ops, tcp r/w				
 *	@date 18.04.17			Added UDP read
 *	@date 22.04.17			Calculate memory base addresses
 *	@date 23.04.17			Changed uint16 to bool/int return type
 *	@date 23.04.17			Added UDP data write, TCP client		*/

#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include "Ethernet.h"
#include "../Debug/debug.h"
#include "../../drivers/W5100/W5100.h"

#define UDP_HEADER_LEN		8

static uint8_t ethSubnetBackup[4];

/*! @brief Utility function to determine the RX/TX memory size
 *         of a socket
 *
 *	@note Only to be used within Ethernet.c
 * 
 *	@param[in] socket		Target socket
 *	@param[in] msr			RMSR or TMSR
 *	@return uint16_t		RX/TX memory size of socket	
 *	@date 22.04.17			First implementation					*/
static uint16_t ethSockGetMemSize(socket_t socket, w51eReg_t msr)
{
	return 0x400 << ((w51eRead(msr) & (0x3 << (socket * 2))) >> (socket * 2));
}

/*!	@brief Utility function to determine RX/TX memory base address
 *
 *	@note Only to be used within Ethernet.c
 *
 *	@param[in] socket		Target socket
 *	@param[in] msr			RMSR or TMSR
 *	@param[in] memBase		Memory base address
 *	@return uint16_t		RX/TX memory base address for socket
 *	@date 22.04.17			First implementation					*/
static uint16_t ethSockGetMemBaseAddr(socket_t socket, w51eReg_t msr)
{
	uint_fast16_t memBase;
	if (msr == W5100_REG_RMSR)
		memBase = W5100_CHIP_BASE + W5100_RXM_BASE;
	else
		memBase = W5100_CHIP_BASE + W5100_TXM_BASE;
	
	for(; socket>0; --socket)
	{
		memBase += ethSockGetMemSize(socket-1, msr);
	}

	return memBase;			
}

/*! @brief (Re)initialize the W5100 NIC
 *
 *	@param[in] txMemSizes	TX Memory size config (TMSR)
 *	@param[in] rxMemSizes	RX Memory size config (RMSR)
 *  @date 10.04.17			First implementation
 *	@date 23.04.17			Custom memory sizes						*/
void ethInit(uint8_t txMemSizes, uint8_t rxMemSizes)
{
	// Reset all W5100 registers
	w51eWrite(W5100_REG_MR, (1 << W5100_MR_RST));

	// Enable socket interrupts 0 through 3
	w51eWrite(W5100_REG_IMR, (1 << W5100_IM_IR0) | (1 << W5100_IM_IR1) | (1 << W5100_IM_IR2) | (1 << W5100_IM_IR3));

	// Set retry time to 200ms
	// default value after reset: 0x07D0

	// Set retry count to 8
	// default value after reset: 0x08

	// Set memory allocation
	w51eWrite(W5100_REG_TMSR, txMemSizes);
	w51eWrite(W5100_REG_RMSR, rxMemSizes);
}

/*! @brief Set MAC and IP Addresses of the W5100 NIC
 *
 *  @param[in] mac[6]		Hardware MAC Address
 *	@param[in] subnet[4]	Subnet mask
 *	@param[in] ip[4]		Local IP Address			
 *	@date 13.12.16			First implementation
 *	@date 23.04.17			Back up subnet mask in RAM				*/
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

	ethSubnetBackup[0] = subnet[0];
	ethSubnetBackup[1] = subnet[1];
	ethSubnetBackup[2] = subnet[2];
	ethSubnetBackup[3] = subnet[3];

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
 *	@return bool			true if successful
 *	@date 10.04.17			First implementation
 *	@date 23.04.17			Bool return type					*/
bool ethSockOpen(socket_t socket, uint16_t port, uint8_t protocol, uint8_t modeFlags)
{
	// Make sure socket is closed, first
	if (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) != W5100_Sn_SR_SOCK_CLOSED)
	{
		ethSockClose(socket);
	}

	// Apply protocol and mode flags
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_MR), (protocol << W5100_Sn_MR_PROTO) | modeFlags);

	// Set source port number
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
		return false;
	}

	// Socket is now open
	return true;
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
 *	@return bool			true if successful
 *	@date 10.04.17			First implementation
 *	@date 23.04.17			Bool return type						*/
bool ethSockListen(socket_t socket)
{
	// LISTEN mode can only be entered in TCP mode after init
	// Check if socket is initialized properly
	if (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) != W5100_Sn_SR_SOCK_INIT)
	{
		ERROR_LOG("Sock not in INIT mode");
		return false;
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
		return false;
	 }

	 // Socket is now in LISTEN mode
	 return true;
}

/*! @brief Connect socket to target in TCP/IP mode
 *	
 *	@param[in] socket		Socket to connect from
 *	@param[in] *targetPeer	Destination to connect to
 *	@return book			true if connection established
 *	@date 23.04.17			First implementation					*/
bool ethSockConnect(socket_t socket, peer_t* targetPeer)
{
	// Check if socket is initialized properly
	if (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) != W5100_Sn_SR_SOCK_INIT)
	{
		ERROR_LOG("Sock not in INIT mode");
		return false;
	}

	// Set destination IP address
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_DIPR0), targetPeer->ip[0]);
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_DIPR1), targetPeer->ip[1]);
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_DIPR2), targetPeer->ip[2]);
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_DIPR3), targetPeer->ip[3]);

	// Set destination port
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_DPORT0), targetPeer->port);

	// Send CONNECT command
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_CONNECT);

	// Wait for established connection
	while (w51eRead(W5100_SRG(socket, W5100_REG_S0_SR)) != W5100_Sn_SR_SOCK_ESTABLISHED)
	{
		if (w51eRead(W5100_SRG(socket, W5100_REG_S0_IR)) == W5100_Sn_IR_TIMEOUT)
		{
			return false;
		}
	}

	return true;
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
 *  @return int				Number of bytes to be read
 *	@date 11.04.17			First implementation
 *	@date 23.04.17			int return type							*/
int ethAvailable(socket_t socket)	
{
	return w51eReadW(W5100_REG_S0_RX_RSR0);
}

/*! @brief Read data from socket receive memory
 *
 *  @param[in] socket		Socket to read from
 *	@param[out] *dataBuffer	Target buffer to store data
 *	@param[in] bufSize		Buffer size
 *	@return int				Number of bytes read
 *	@date 11.04.17			First implementation
 *	@date 22.04.17			Use getMemSize and getBaseAddr
 *	@date 23.04.17			int return type							*/
int ethRead(socket_t socket, uint8_t* dataBuffer, int bufSize)
{
	// Check if data is available for reading
	uint16_t bytesAvailable = ethAvailable(socket);
	if (bytesAvailable == 0)
	{
		// Abort, if no data queued
		return 0;
	}

	// Get read pointer and mask from device 
	uint16_t readStart = w51eReadW(W5100_SRG(socket, W5100_REG_S0_RX_RD0));
	uint16_t mask = ethSockGetMemSize(socket, W5100_REG_RMSR) - 1;
	uint16_t baseAddr = ethSockGetMemBaseAddr(socket, W5100_REG_RMSR);

	// Cyclic read single byte until all data is processed or buffer limit is reached
	uint_fast16_t dataCounter = 0;
	while ((dataCounter < bufSize) && (dataCounter < bytesAvailable))
	{
		dataBuffer[dataCounter] = w51eRead(baseAddr + (readStart & mask));
		++readStart;
		++dataCounter;
	}

	// Set up read pointer for next receive operation
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_RX_RD0), readStart);

	// Incoming data will now be stored starting at the RXD address
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_RECV);

	return dataCounter;
}

/*!	@brief Read data from non TCP/IP socket
 *
 *	@note Currently UDP-only.
 *
 *	@param[in] socket		Socket to read from
 *	@param[out] *sourcePeer	Sender info
 *	@param[out]	*dataBuffer	Target buffer to store data in
 *	@param[in] bufSize		Maximum data buffer size
 *	@return int				Number of bytes read, or -1 on error
 *	@date 23.04.17			First implementation					*/
int ethReadFrom(socket_t socket, peer_t* sourcePeer, uint8_t* dataBuffer, int bufSize)
{
	// Minimum of 8 header bytes
	uint16_t bytesAvailable = ethAvailable(socket);
	if (bytesAvailable < 8)
	{
		return 0;
	}

	// Get device memory addresses
	uint16_t readStart = w51eReadW(W5100_SRG(socket, W5100_REG_S0_RX_RD0));
	uint16_t mask = ethSockGetMemSize(socket, W5100_REG_RMSR) - 1;
	uint16_t baseAddr = ethSockGetMemBaseAddr(socket, W5100_REG_RMSR);

	// Setup data and frame counters
	uint_fast16_t dataCounter = 0;
	uint_fast16_t frameLen;

	// Fetch header data
	switch (w51eRead(W5100_SRG(socket, W5100_REG_S0_MR)) & 0x07)
	{
		case W5100_Sn_MR_PROTO_UDP:
		{ 
			// Read UDP header
			uint8_t header[UDP_HEADER_LEN];
			while (dataCounter < UDP_HEADER_LEN)
			{
				header[dataCounter] = w51eRead(baseAddr + (readStart & mask));
				++readStart;
				++dataCounter;
			}

			// Extract Peer IP and port from header
			// as per W5100 datasheet 5.2.2 UDP (p.52)
			sourcePeer->ip[0] = header[0];
			sourcePeer->ip[1] = header[1];
			sourcePeer->ip[2] = header[2];
			sourcePeer->ip[3] = header[3];
			sourcePeer->port = (header[4] << 8) | header[5];

			// Extract data frame length from header
			frameLen = (header[6] << 8) | header[7];
		}
		break;
		/*case W5100_Sn_MR_PROTO_IPRAW:
		{
			uint8_t header[IPRAW_HEADER_LEN];
			while (dataCounter < IPRAW_HEADER_LEN)
			{
				header[dataCounter] = w51eRead(W5100_CHIP_BASE + W5100_RXM_BASE + (readStart & mask));
				++readStart;
				++dataCounter;
			}

			// Extract peer IP from header
			// as per W5100 datasheet 5.2.3 IPRAW (p.58)
			sourcePeer->ip[0] = header[0];
			sourcePeer->ip[1] = header[1];
			sourcePeer->ip[2] = header[2];
			sourcePeer->ip[3] = header[3];

			// Extract frame length
			frameLen = (header[4] << 8) | header[5];
		}
		break;*/
		default:
			// IPRAW / PPPoE / MACRAW not supported
			frameLen = 0;
	}

	// Calculate address of last data byte
	uint16_t readEnd = readStart + frameLen;

	// Read frame data
	dataCounter = 0;
	while ((dataCounter < frameLen) && (dataCounter < bufSize) && (dataCounter < (bytesAvailable - 8)))
	{
		dataBuffer[dataCounter] = w51eRead(baseAddr + (readStart & mask));
		++readStart;
		++dataCounter;
	}

	// Set up read pointer for next receive operation, skip remaining data
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_RX_RD0), readEnd);

	// Incoming data will now be stored starting at the RXD address
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_RECV);

	return dataCounter;
}

/*! @brief Write data to socket memory for transmission
 *
 *	@param[in] socket		Target socket
 *	@param[in] *dataBuffer	Buffer to read data from
 *	@param[in] dataLenght	Number of bytes to be sent
 *	@return int				Number of bytes actually written
 *	@date 14.04.17			First implementation					
 *	@date 22.04.17			Use getMemSize and getBaseAddr			*/
int ethWrite(socket_t socket, uint8_t* dataBuffer, int dataLength)
{
	// Abort if no data to be written
	if (dataLength == 0)
	{
		return 0;
	}

	// Abort if device transmit memory is full
	int freeSize = w51eReadW(W5100_SRG(socket, W5100_REG_S0_TX_FSR0));
	if (freeSize == 0)
	{
		ERROR_LOG("TXM full");
		return 0;
	}

	// Get send pointer and mask from device
	uint16_t writeStart = w51eReadW(W5100_SRG(socket, W5100_REG_S0_TX_WR0));
	uint16_t mask = ethSockGetMemSize(socket, W5100_REG_TMSR) - 1;
	uint16_t baseAddr = ethSockGetMemBaseAddr(socket, W5100_REG_TMSR);

	// Cyclic write single byte until all data is processed or buffer limit is reached
	uint_fast16_t dataCounter = 0;
	while ((dataCounter < dataLength) && (dataCounter < freeSize))
	{
		w51eWrite(baseAddr + ((writeStart + dataCounter) & mask), dataBuffer[dataCounter]);
		++dataCounter;
	}

	// Set up write pointer for next transmit operation
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_TX_WR0), (writeStart + dataCounter));

	// Trigger sending data till new TXR address
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_SEND);

	// Wait until data is sent
	uint_fast16_t timeout = 0;
	while (!(w51eRead(W5100_SRG(socket, W5100_REG_S0_IR)) & W5100_Sn_IR_SEND_OK) && (timeout < UINT_FAST16_MAX)) 
	{
		++timeout;
	}
	if (timeout == UINT_FAST16_MAX)
	{
		// Timeout error
		ERROR_LOG("Timeout sending data");
		return 0;
	}

	return dataCounter;
}

/*!	@brief Write data to non-TCP/IP socket for transmission
 *
 *	@param[in] socket		Socket to send from
 *	@param[in] *targetPeer	Target IP+port
 *	@param[in] *dataBuffer	Source data buffer
 *	@param[in] dataLength	Number of bytes to send from buffer
 *	@return int				Actual number of bytes sent
 *	@date 23.04.17			First implementation					*/
int ethWriteTo(socket_t socket, peer_t* targetPeer, uint8_t* dataBuffer, int dataLength)
{
	// Abort if no data to be written
	if (dataLength == 0)
	{
		return 0;
	}

	// Set destination IP address
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_DIPR0), targetPeer->ip[0]);
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_DIPR1), targetPeer->ip[1]);
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_DIPR2), targetPeer->ip[2]);
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_DIPR3), targetPeer->ip[3]);

	// Set destination port
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_DPORT0), targetPeer->port);

	// Abort if device transmit memory is full
	int freeSize = w51eReadW(W5100_SRG(socket, W5100_REG_S0_TX_FSR0));
	if (freeSize == 0)
	{
		ERROR_LOG("TXM full");
		return 0;
	}

	// Get send pointer and mask from device
	uint16_t writeStart = w51eReadW(W5100_SRG(socket, W5100_REG_S0_TX_WR0));
	uint16_t mask = ethSockGetMemSize(socket, W5100_REG_TMSR) - 1;
	uint16_t baseAddr = ethSockGetMemBaseAddr(socket, W5100_REG_TMSR);

	// Cyclic write single byte until all data is processed or buffer limit is reached
	uint_fast16_t dataCounter = 0;
	while ((dataCounter < dataLength) && (dataCounter < freeSize))
	{
		w51eWrite(baseAddr + ((writeStart + dataCounter) & mask), dataBuffer[dataCounter]);
		++dataCounter;
	}
	
	// Set up write pointer for next transmit operation
	w51eWriteW(W5100_SRG(socket, W5100_REG_S0_TX_WR0), (writeStart + dataCounter));

	// Handle hardware bug where the NIC would send an 
	// invalid ARP reply if the target's IP address is
	// "0.0.0.0".
	// See Erratasheet p.9 (Erratum 2)
	if ((targetPeer->ip[0] == 0) && (targetPeer->ip[1] == 0) && (targetPeer->ip[2] == 0) && (targetPeer->ip[3] == 0))
	{
		w51eWrite(W5100_REG_SUBR0, 0x00);
		w51eWrite(W5100_REG_SUBR1, 0x00);
		w51eWrite(W5100_REG_SUBR2, 0x00);
		w51eWrite(W5100_REG_SUBR3, 0x00);
	}

	// Trigger sending data till new TXR address
	w51eWrite(W5100_SRG(socket, W5100_REG_S0_CR), W5100_Sn_CR_SEND);

	// Wait until data is sent
	uint_fast16_t timeout = 0;
	while (!(w51eRead(W5100_SRG(socket, W5100_REG_S0_IR)) & W5100_Sn_IR_SEND_OK) && (timeout < UINT_FAST16_MAX))
	{
		++timeout;
	}
	if (timeout == UINT_FAST16_MAX)
	{
		// A timeout may be caused by a hardware bug where
		// TX_RD and TX_WR will never equal. Socket must be
		// reset in this case
		
		// Timeout error
		ERROR_LOG("Timeout sending data");
		ethSockClose(socket);
		return 0;
	}

	// Re-apply previous Subnet mask, if it was
	// reset to handle hardware bug (see above).
	if ((targetPeer->ip[0] == 0) && (targetPeer->ip[1] == 0) && (targetPeer->ip[2] == 0) && (targetPeer->ip[3] == 0))
	{
		w51eWrite(W5100_REG_SUBR0, ethSubnetBackup[0]);
		w51eWrite(W5100_REG_SUBR1, ethSubnetBackup[1]);
		w51eWrite(W5100_REG_SUBR2, ethSubnetBackup[2]);
		w51eWrite(W5100_REG_SUBR3, ethSubnetBackup[3]);
	}

	return dataLength;
}