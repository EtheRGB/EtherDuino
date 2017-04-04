/*
 * debug.h
 *
 * Created: 12/7/2016 9:01:47 PM
 *  Author: inselc
 */ 


#ifndef DEBUG_H_
#define DEBUG_H_

#include "../../core/Serial/Serial.h"

#define DEBUG_LOG(x) serialWriteStr("\r\n -- DEBUG . "); \
					 serialWriteStr(x); \
					 serialWriteStr(" --\r\n")

#endif /* DEBUG_H_ */