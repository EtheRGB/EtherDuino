/*! @brief Data exchange over Ethernet
 *
 *	@author inselc
 *	@date 13.12.16			Initial version
 *	@date 10.04.17			Added socket ops, tcp r/w				
 *	@date 18.04.17			Added udp, ipraw r/w
 *	@date 23.04.17			Changed uint16 to bool/int
 *	@date 23.04.17			Added peer_t type						*/

#ifndef ETHERNET_H_
#define ETHERNET_H_

/*! @file */

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t socket_t;

typedef struct {
	uint8_t ip[4];
	uint16_t port;
} peer_t;

void ethInit(uint8_t txMemSizes, uint8_t rxMemSizes);
void ethSetLocalIP(uint8_t mac[6], uint8_t subnet[4], uint8_t ip[4]);

bool ethSockOpen(socket_t socket, uint16_t port, uint8_t protocol, uint8_t modeFlags);
void ethSockClose(socket_t socket);

bool ethSockListen(socket_t socket);
bool ethSockConnect(socket_t socket, peer_t* targetPeer);
void ethSockDisconnect(socket_t socket);

int ethAvailable(socket_t socket);
int ethRead(socket_t socket, uint8_t* dataBuffer, int bufSize);
int ethReadFrom(socket_t socket, peer_t* sourcePeer, uint8_t* dataBuffer, int bufSize);
int ethWrite(socket_t socket, uint8_t* dataBuffer, int dataLength);
int ethWriteTo(socket_t socket, peer_t* targetPeer, uint8_t* dataBuffer, int dataLength);

#endif /* ETHERNET_H_ */