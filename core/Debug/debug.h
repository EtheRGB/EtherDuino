/*
 * debug.h
 *
 * Created: 12/7/2016 9:01:47 PM
 *  Author: inselc
 */ 

#ifndef DEBUG_H_
#define DEBUG_H_

#include "../../core/Serial/Serial.h"
#include <stdlib.h>

static inline void ERROR_LOG(const char* x) 
{
	serialWriteStr("\r\n Error in " __FILE__ ":");
	char n[4];
	itoa(__LINE__, n, 10);
	serialWriteBuf((uint8_t*)n, 4, -1);
	serialWriteStr(" ");
	serialWriteStr(x);
	serialWriteStr("\r\n");
}

#endif /* DEBUG_H_ */