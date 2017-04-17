/*! @brief Wiznet W5100 Ethernet Controller driver headers
 *
 *	@author	inselc
 *	@date	07.12.16		initial version							*/ 


#ifndef W5100_H_
#define W5100_H_

#include <stdint.h>

/* @file */

#define W5100_MR_RST	7	/* MR Reset */
#define W5100_MR_PB		4	/* MR Ping block mode */
#define W5100_MR_PPPoE	3	/* MR PPPoE mode */
#define W5100_MR_AI		1	/* MR Address auto-increment in Bus I/F */
#define W5100_MR_IND	0	/* MR Indirect Bus I/F mode */

#define W5100_IR_CONFLICT	7	/* IR Conflict flag */
#define W5100_IR_UNREACH	6	/* IR Unreachable flag */
#define W5100_IR_PPPoE		5	/* IR PPPoE conn close flag */
#define W5100_IR_S3_INT		3	/* IR Socket 3 interrupt flag */
#define W5100_IR_S2_INT		2	/* IR Socket 2 interrupt flag */
#define W5100_IR_S1_INT		1	/* IR Socket 1 interrupt flag */
#define W5100_IR_S0_INT		0	/* IR Socket 0 interrupt flag */

#define W5100_IM_IR7	7	/* IM Conflict int enable */
#define W5100_IM_IR6	6	/* IM Destination unreachable int enable */
#define W5100_IM_IR5	5	/* IM PPPoE close int enable */
#define W5100_IM_IR3	3	/* IM Socket 3 int enable */
#define W5100_IM_IR2	2	/* IM Socket 2 int enable */
#define W5100_IM_IR1	1	/* IM Socket 1 int enable */
#define W5100_IM_IR0	0	/* IM Socket 0 int enable */

#define W5100_RTR_FACTOR_100us 1	/* LSB = 100us */

#define W5100_MSR_S0	0	/* Mem Size Socket 0 */
#define W5100_MSR_S1	2	/* Mem Size Socket 0 */
#define W5100_MSR_S2	4	/* Mem Size Socket 0 */
#define W5100_MSR_S3	6	/* Mem Size Socket 0 */
#define W5100_MSR_1K		0x00
#define W5100_MSR_2K		0x01
#define W5100_MSR_4K		0x02
#define W5100_MSR_8K		0x03

#define W5100_PATR_PAP	0xC023
#define W5100_PATR_CHAP	0xC223

#define W5100_PTIMER_FACTOR_25ms	1	/* LSB = 25ms */

#define W5100_Sn_MR_MULTI	7	/* Socket n Multicast enable */
#define W5100_Sn_MR_MF		6	/* Socket n MAC filter enable */
#define W5100_Sn_MR_NDMC	5	/* Socket n Use No Delayed ACK */
#define W5100_Sn_MR_PROTO	0	/* Socket n Protocol */
#define W5100_Sn_MR_PROTO_Closed	0x00
#define W5100_Sn_MR_PROTO_TCP		0x01
#define W5100_Sn_MR_PROTO_UDP		0x02
#define W5100_Sn_MR_PROTO_IPRAW		0x03
#define W5100_Sn_MR_PROTO_MACRAW	0x04 /* Socket 0 only */
#define W5100_Sn_MR_PROTO_PPPoE		0x05 /* Socket 0 only */

#define W5100_Sn_CR_OPEN	0x01
#define W5100_Sn_CR_LISTEN	0x02
#define W5100_Sn_CR_CONNECT	0x04
#define W5100_Sn_CR_DISCON	0x08
#define W5100_Sn_CR_CLOSE	0x10
#define W5100_Sn_CR_SEND	0x20
#define W5100_Sn_CR_SEND_MAC 0x21
#define W5100_Sn_CR_SEND_KEEP 0x22
#define W5100_Sn_CR_RECV	0x40

#define W5100_Sn_IR_SEND_OK	4	/* Send completed flag */
#define W5100_Sn_IR_TIMEOUT	3
#define W5100_Sn_IR_RECV	2	/* Receiveing data */
#define W5100_Sn_IR_DISCON	1	/* Connection terminated */
#define W5100_Sn_IR_CON		0	/* Connection established */

#define W5100_Sn_SR_SOCK_CLOSED 0x00
#define W5100_Sn_SR_SOCK_INIT	0x13
#define W5100_Sn_SR_SOCK_LISTEN 0x14
#define W5100_Sn_SR_SOCK_ESTABLISHED 0x17
#define W5100_Sn_SR_SOCK_CLOSE_WAIT	0x1C
#define W5100_Sn_SR_SOCK_UDP	0x22
#define W5100_Sn_SR_SOCK_IPRAW	0x32
#define W5100_Sn_SR_SOCK_MACRAW 0x42
#define W5100_Sn_SR_SOCK_PPPOE	0x5F

#define W5100_Sn_SR_SOCK_SYNSENT 0x15
#define W5100_Sn_SR_SOCK_SYNRECV 0x16
#define W5100_Sn_SR_SOCK_FIN_WAIT 0x18
#define W5100_Sn_SR_SOCK_CLOSING 0x1A
#define W5100_Sn_SR_SOCK_TIME_WAIT 0x1B
#define W5100_Sn_SR_SOCK_LAST_ACK 0x1D
#define W5100_Sn_SR_SOCK_ARP	 0x01

#define W5100_CHIP_BASE 0x0000
#define W5100_TXM_BASE	0x4000
#define W5100_TXM_END	0x5FFF
#define W5100_RXM_BASE	0x6000
#define W5100_RXM_END	0x7FFF

typedef enum tagW51eReg_t{	
	/* -- Common registers -- */
	/* Mode */
	W5100_REG_MR	= 0x0000,
	/* Gateway address */
	W5100_REG_GAR0	= 0x0001,
	W5100_REG_GAR1	= 0x0002,
	W5100_REG_GAR2	= 0x0003,
	W5100_REG_GAR3	= 0x0004,
	/* Subnet mask address */
	W5100_REG_SUBR0	= 0x0005,
	W5100_REG_SUBR1 = 0x0006,
	W5100_REG_SUBR2 = 0x0007,
	W5100_REG_SUBR3 = 0x0008,
	/* Source hardware address (MAC) */
	W5100_REG_SHAR0 = 0x0009,
	W5100_REG_SHAR1 = 0x000A,
	W5100_REG_SHAR2 = 0x000B,
	W5100_REG_SHAR3 = 0x000C,
	W5100_REG_SHAR4 = 0x000D,
	W5100_REG_SHAR5 = 0x000E,
	/* Source IP address */
	W5100_REG_SIPR0	= 0x000F,
	W5100_REG_SIPR1	= 0x0010,
	W5100_REG_SIPR2 = 0x0011,
	W5100_REG_SIPR3	= 0x0012,
	/* reserved 0x0013 - 0x0014 */
	/* Interrupt */
	W5100_REG_IR	= 0x0015,
	/* Interrupt mask */
	W5100_REG_IMR	= 0x0016,
	/* Retry time */
	W5100_REG_RTR0	= 0x0017,
	W5100_REG_RTR1	= 0x0018,
	/* Retry count */
	W5100_REG_RCR	= 0x0019,
	/* RX memory size */
	W5100_REG_RMSR	= 0x001A,
	/* TX memory size */
	W5100_REG_TMSR	= 0x001B,
	/* PPPoE Authentication Type */
	W5100_REG_PATR0	= 0x001C,
	W5100_REG_PATR1	= 0x001D,
	/* reserved 0x001E - 0x0027 */
	/* PPP LCP Request Timer */
	W5100_REG_PTIMER= 0x0028,
	/* PPP LCP Magic Number */
	W5100_REG_PMAGIC= 0x0029,
	/* Unreachable IP address */
	W5100_REG_UIPR0	= 0x002A,
	W5100_REG_UIPR1 = 0x002B,
	W5100_REG_UIPR2 = 0x002C,
	W5100_REG_UIPR3 = 0x002D,
	/* Unreachable Port */
	W5100_REG_UPORT0= 0x002E,
	W5100_REG_UPORT1= 0x002F,
	/* reserved 0x0030 - 0x03FF */
	
	/* -- Socket 0 registers -- */
	/* Socket 0 Mode */
	W5100_REG_S0_MR	= 0x0400,
	/* Socket 0 Command */
	W5100_REG_S0_CR	= 0x0401,
	/* Socket 0 Interrupt */
	W5100_REG_S0_IR	= 0x0402,
	/* Socket 0 Status */
	W5100_REG_S0_SR	= 0x0403,
	/* Socket 0 Source Port */
	W5100_REG_S0_PORT0 = 0x0404,
	W5100_REG_S0_PORT1 = 0x0405,
	/* Socket 0 Destination Hardware Address */
	W5100_REG_S0_DHAR0 = 0x0406,
	W5100_REG_S0_DHAR1 = 0x0407,
	W5100_REG_S0_DHAR2 = 0x0408,
	W5100_REG_S0_DHAR3 = 0x0409,
	W5100_REG_S0_DHAR4 = 0x040A,
	W5100_REG_S0_DHAR5 = 0x040B,
	/* Socket 0 Destination IP Address */
	W5100_REG_S0_DIPR0 = 0x040C,
	W5100_REG_S0_DIPR1 = 0x040D,
	W5100_REG_S0_DIPR2 = 0x040E,
	W5100_REG_S0_DIPR3 = 0x040F,
	/* Socket 0 Destination Port */
	W5100_REG_S0_DPORT0 = 0x0410,
	W5100_REG_S0_DPORT1 = 0x0411,
	/* Socket 0 max. segment size */
	W5100_REG_S0_MSSR0 = 0x0412,
	W5100_REG_S0_MSSR1 = 0x0413,
	/* Socket 0 Protocol in IP Raw Mode */
	W5100_REG_S0_PROTO = 0x0414,
	/* Socket 0 IP TOS */
	W5100_REG_S0_TOS = 0x0415,
	/* Socket 0 IP TTL */
	W5100_REG_S0_TTL = 0x0416,
	/* reserved 0x0417 - 0x041F */
	/* Socket 0 TX free size */
	W5100_REG_S0_TX_FSR0 = 0x0420,
	W5100_REG_S0_TX_FSR1 = 0x0421,
	/* Socket 0 TX read pointer */
	W5100_REG_S0_TX_RD0 = 0x0422,
	W5100_REG_S0_TX_RD1 = 0x0423,
	/* Socket 0 TX write pointer */
	W5100_REG_S0_TX_WR0 = 0x0424,
	W5100_REG_S0_TX_WR1 = 0x0425,
	/* Socket 0 RX received size */
	W5100_REG_S0_RX_RSR0 = 0x0426,
	W5100_REG_S0_RX_RSR1 = 0x0427,
	/* Socket 0 RX read pointer */ 
	W5100_REG_S0_RX_RD0 = 0x0428,
	W5100_REG_S0_RX_RD1 = 0x0429,
	/* reserved 0x042A - 0x042B */
	/* reserved 0x042C - 0x04FF */

	/* -- Socket 11 registers -- */
	/* Socket 11 Mode */
	W5100_REG_S1_MR	= 0x0500,
	/* Socket 11 Command */
	W5100_REG_S1_CR	= 0x0501,
	/* Socket 11 Interrupt */
	W5100_REG_S1_IR	= 0x0502,
	/* Socket 11 Status */
	W5100_REG_S1_SR	= 0x0503,
	/* Socket 11 Source Port */
	W5100_REG_S1_PORT0 = 0x0504,
	W5100_REG_S1_PORT1 = 0x0505,
	/* Socket 11 Destination Hardware Address */
	W5100_REG_S1_DHAR0 = 0x0506,
	W5100_REG_S1_DHAR1 = 0x0507,
	W5100_REG_S1_DHAR2 = 0x0508,
	W5100_REG_S1_DHAR3 = 0x0509,
	W5100_REG_S1_DHAR4 = 0x050A,
	W5100_REG_S1_DHAR5 = 0x050B,
	/* Socket 11 Destination IP Address */
	W5100_REG_S1_DIPR0 = 0x050C,
	W5100_REG_S1_DIPR1 = 0x050D,
	W5100_REG_S1_DIPR2 = 0x050E,
	W5100_REG_S1_DIPR3 = 0x050F,
	/* Socket 11 Destination Port */
	W5100_REG_S1_DPORT0 = 0x0510,
	W5100_REG_S1_DPORT1 = 0x0511,
	/* Socket 11 max. segment size */
	W5100_REG_S1_MSSR0 = 0x0512,
	W5100_REG_S1_MSSR1 = 0x0513,
	/* Socket 11 Protocol in IP Raw Mode */
	W5100_REG_S1_PROTO = 0x0514,
	/* Socket 11 IP TOS */
	W5100_REG_S1_TOS = 0x0515,
	/* Socket 11 IP TTL */
	W5100_REG_S1_TTL = 0x0516,
	/* reserved 0x0517 - 0x051F */
	/* Socket 11 TX free size */
	W5100_REG_S1_TX_FSR0 = 0x0520,
	W5100_REG_S1_TX_FSR1 = 0x0521,
	/* Socket 11 TX read pointer */
	W5100_REG_S1_TX_RD0 = 0x0522,
	W5100_REG_S1_TX_RD1 = 0x0523,
	/* Socket 11 TX write pointer */
	W5100_REG_S1_TX_WR0 = 0x0524,
	W5100_REG_S1_TX_WR1 = 0x0525,
	/* Socket 11 RX received size */
	W5100_REG_S1_RX_RSR0 = 0x0526,
	W5100_REG_S1_RX_RSR1 = 0x0527,
	/* Socket 11 RX read pointer */
	W5100_REG_S1_RX_RD0 = 0x0528,
	W5100_REG_S1_RX_RD1 = 0x0529,
	/* reserved 0x052A - 0x052B */
	/* reserved 0x052C - 0x05FF */

	/* -- Socket 2 registers -- */
	/* Socket 2 Mode */
	W5100_REG_S2_MR	= 0x0600,
	/* Socket 2 Command */
	W5100_REG_S2_CR	= 0x0601,
	/* Socket 2 Interrupt */
	W5100_REG_S2_IR	= 0x0602,
	/* Socket 2 Status */
	W5100_REG_S2_SR	= 0x0603,
	/* Socket 2 Source Port */
	W5100_REG_S2_PORT0 = 0x0604,
	W5100_REG_S2_PORT1 = 0x0605,
	/* Socket 2 Destination Hardware Address */
	W5100_REG_S2_DHAR0 = 0x0606,
	W5100_REG_S2_DHAR1 = 0x0607,
	W5100_REG_S2_DHAR2 = 0x0608,
	W5100_REG_S2_DHAR3 = 0x0609,
	W5100_REG_S2_DHAR4 = 0x060A,
	W5100_REG_S2_DHAR5 = 0x060B,
	/* Socket 2 Destination IP Address */
	W5100_REG_S2_DIPR0 = 0x060C,
	W5100_REG_S2_DIPR1 = 0x060D,
	W5100_REG_S2_DIPR2 = 0x060E,
	W5100_REG_S2_DIPR3 = 0x060F,
	/* Socket 2 Destination Port */
	W5100_REG_S2_DPORT0 = 0x0610,
	W5100_REG_S2_DPORT1 = 0x0611,
	/* Socket 2 max. segment size */
	W5100_REG_S2_MSSR0 = 0x0612,
	W5100_REG_S2_MSSR1 = 0x0613,
	/* Socket 2 Protocol in IP Raw Mode */
	W5100_REG_S2_PROTO = 0x0614,
	/* Socket 2 IP TOS */
	W5100_REG_S2_TOS = 0x0615,
	/* Socket 2 IP TTL */
	W5100_REG_S2_TTL = 0x0616,
	/* reserved 0x0617 - 0x061F */
	/* Socket 2 TX free size */
	W5100_REG_S2_TX_FSR0 = 0x0620,
	W5100_REG_S2_TX_FSR1 = 0x0621,
	/* Socket 2 TX read pointer */
	W5100_REG_S2_TX_RD0 = 0x0622,
	W5100_REG_S2_TX_RD1 = 0x0623,
	/* Socket 2 TX write pointer */
	W5100_REG_S2_TX_WR0 = 0x0624,
	W5100_REG_S2_TX_WR1 = 0x0625,
	/* Socket 2 RX received size */
	W5100_REG_S2_RX_RSR0 = 0x0626,
	W5100_REG_S2_RX_RSR1 = 0x0627,
	/* Socket 2 RX read pointer */
	W5100_REG_S2_RX_RD0 = 0x0628,
	W5100_REG_S2_RX_RD1 = 0x0629,
	/* reserved 0x062A - 0x062B */
	/* reserved 0x062C - 0x06FF */

	/* -- Socket 3 registers -- */
	/* Socket 3 Mode */
	W5100_REG_S3_MR	= 0x0700,
	/* Socket 3 Command */
	W5100_REG_S3_CR	= 0x0701,
	/* Socket 3 Interrupt */
	W5100_REG_S3_IR	= 0x0702,
	/* Socket 3 Status */
	W5100_REG_S3_SR	= 0x0703,
	/* Socket 3 Source Port */
	W5100_REG_S3_PORT0 = 0x0704,
	W5100_REG_S3_PORT1 = 0x0705,
	/* Socket 3 Destination Hardware Address */
	W5100_REG_S3_DHAR0 = 0x0706,
	W5100_REG_S3_DHAR1 = 0x0707,
	W5100_REG_S3_DHAR2 = 0x0708,
	W5100_REG_S3_DHAR3 = 0x0709,
	W5100_REG_S3_DHAR4 = 0x070A,
	W5100_REG_S3_DHAR5 = 0x070B,
	/* Socket 3 Destination IP Address */
	W5100_REG_S3_DIPR0 = 0x070C,
	W5100_REG_S3_DIPR1 = 0x070D,
	W5100_REG_S3_DIPR2 = 0x070E,
	W5100_REG_S3_DIPR3 = 0x070F,
	/* Socket 3 Destination Port */
	W5100_REG_S3_DPORT0 = 0x0710,
	W5100_REG_S3_DPORT1 = 0x0711,
	/* Socket 3 max. segment size */
	W5100_REG_S3_MSSR0 = 0x0712,
	W5100_REG_S3_MSSR1 = 0x0713,
	/* Socket 3 Protocol in IP Raw Mode */
	W5100_REG_S3_PROTO = 0x0714,
	/* Socket 3 IP TOS */
	W5100_REG_S3_TOS = 0x0715,
	/* Socket 3 IP TTL */
	W5100_REG_S3_TTL = 0x0716,
	/* reserved 0x0717 - 0x071F */
	/* Socket 3 TX free size */
	W5100_REG_S3_TX_FSR0 = 0x0720,
	W5100_REG_S3_TX_FSR1 = 0x0721,
	/* Socket 3 TX read pointer */
	W5100_REG_S3_TX_RD0 = 0x0722,
	W5100_REG_S3_TX_RD1 = 0x0723,
	/* Socket 3 TX write pointer */
	W5100_REG_S3_TX_WR0 = 0x0724,
	W5100_REG_S3_TX_WR1 = 0x0725,
	/* Socket 3 RX received size */
	W5100_REG_S3_RX_RSR0 = 0x0726,
	W5100_REG_S3_RX_RSR1 = 0x0727,
	/* Socket 3 RX read pointer */
	W5100_REG_S3_RX_RD0 = 0x0728,
	W5100_REG_S3_RX_RD1 = 0x0729,
	/* reserved 0x072A - 0x072B */
	/* reserved 0x072C - 0x07FF */
} w51eReg_t;

#define W5100_SRG(s,r) ((r) + ((s)<<8))

// Low level Device-specific implementation 
void w51eInit(void);
void w51eWrite(w51eReg_t reg, uint8_t data);
uint8_t w51eRead(w51eReg_t reg);

/*! @brief Read 16-bit data (WORD) from W5100 registers
 *
 *	@param[in] reg16		MSB-register of 2-Byte data
 *	@return uint16_t		Data from registers
 *	@date 14.04.17			First implementation					*/
static inline uint16_t w51eReadW(w51eReg_t reg16)
{
	return ((w51eRead(reg16) & 0x00FF) << 8) | w51eRead(reg16 + 1);
}

/*! @brief Write 16-bit data (WORD) to W5100 register
 *
 *  @param[in] reg16		W5100 MSB-register
 *	@param[in] data			Data to be written
 *	@date 14.04.17			First implementation					*/
static inline void w51eWriteW(w51eReg_t reg16, uint16_t data)
{
	w51eWrite(reg16, (data&0xFF00) >> 8);
	w51eWrite(reg16+1, data&0x00FF);
}

#endif /* W5100_H_ */