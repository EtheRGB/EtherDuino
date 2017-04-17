/*
 * stringtools.c
 *
 * Created: 12/11/2016 3:28:43 AM
 *  Author: inselc
 */ 

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "stringtools.h"

/*! @brief Convert byte into Hexadecimal string
 *
 *	@param[out] *target			Target string buffer (2 chars)
 *	@param[in]	source			Source number (byte)
 *	@return int					String length (2)
 *	@date 11.12.16				first implementation				*/
int hexToStr(char* target, uint8_t source)
{
	int i;
	char c;
	uint8_t n;
	
	if (target == NULL)
	{
		return -1;
	}

	for (i = 0; i <= 1; i++)
	{
		n = (source >> ((1-i) * 4)) & 0x0F;
		
		if (n < 0xA)
			c = '0' + n;	// 0 ... 9
		else
			c = 'A' + (n - 0x0A);	// A ... F
		
		target[i] = c;
	}

	return 2;
}