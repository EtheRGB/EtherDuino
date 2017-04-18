/*
 * Ethernet.h
 *
 * Created: 12/11/2016 3:21:41 AM
 *  Author: inselc
 */ 

#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <stdint.h>

typedef uint8_t socket_t;

void ethInit(void);
void ethSetLocalIP(uint8_t mac[6], uint8_t subnet[4], uint8_t ip[4]);

uint8_t ethSockOpen(socket_t socket, uint16_t port, uint8_t protocol, uint8_t modeFlags);
void ethSockClose(socket_t socket);

uint8_t ethSockListen(socket_t socket);
uint8_t ethSockConnect(socket_t socket, uint8_t host, uint8_t port);
void ethSockDisconnect(socket_t socket);

uint16_t ethAvailable(socket_t socket);
uint16_t ethRead(socket_t socket, uint8_t* dataBuffer, uint16_t bufSize);
uint16_t ethReadFrom(socket_t socket, uint8_t srcIP[4], uint16_t port, uint8_t* dataBuffer, uint16_t bufSize);
uint16_t ethWrite(socket_t socket, uint8_t* dataBuffer, uint16_t dataLength);
uint16_t ethWriteTo(socket_t socket, uint8_t destIP[4], uint16_t port, uint8_t* dataBuffer, uint16_t dataLength);

#endif /* ETHERNET_H_ */