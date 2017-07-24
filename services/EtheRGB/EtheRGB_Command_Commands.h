/*!	@brief EtheRGB Command module command function 
 *	       declarations
 *
 *	@author	inselc
 *	@date 13.07.17			Moved from EtheRGB_Command
 *	@date 18.07.17			Added GroupColor commands
 *	@date 23.07.17			Added SetIP and Reboot					*/

#ifndef ETHERGB_COMMAND_COMMANDS_H_
#define ETHERGB_COMMAND_COMMANDS_H_

// Will return true, if data needs to be sent back
typedef bool (*etheRgbCommandFunc_t)(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer);

bool Command_Test(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer);
bool Command_SetChannelValue(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer);
bool Command_FadeChannelValue(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer);
bool Command_SetGroupColor(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer);
bool Command_SetIpAddress(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer);
bool Command_Reboot(etheRgbCommand_t* commandBuffer, etheRgbCommand_t* responseBuffer);

#endif /* ETHERGB_COMMAND_COMMANDS_H_ */