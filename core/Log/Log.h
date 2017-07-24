/*!	@brief System log messages
 *
 *	@author	inselc
 *	@date 11.07.17		First implementation
 *	@date 23.07.17		String helpers moved to stringtools			*/

#ifndef LOG_H_
#define LOG_H_

/*!	@file */

#include <avr/pgmspace.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

extern const char* SRC_SYSTEM;
extern const char* SRC_ETHERNET;
extern const char* SRC_ETHERGB;

#define LOG_MESSAGE(src,msg)	serialWriteStr(src); serialWriteStrP(PSTR("\t" msg "\r\n"))
#define LOG_ERROR(src,msg)		serialWriteStr(src); serialWriteStrP(PSTR("\tError in " __FILE__ ":" STR(__LINE__) "\t")); serialWriteStrP(PSTR(msg "\r\n"))
#define LOG_CRASH(src,msg)		LOG_ERROR(src,msg); while(1);

#endif /* LOG_H_ */