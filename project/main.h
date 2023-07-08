#ifndef __C8051F020__
#define __C8051F020__

#include "C8051F020.h"


#define SYSCLK			22118400L	//22.1184Mhz
#define SARCLK			2500000L	//2.5Mhz  
#define T4CLK			22118L		//1[Khz]
#define BAUDRATE		230400L			//uart   clock frequency [bps]
// #define BAUDRATE		115200L			//uart   clock frequency [bps]
#define INTERRUPT_TIMER4 	16	//interrupt index
#define INTERRUPT_TIMER2 	5	//interrupt index
#define INTERRUPT_TIMER1 	3	//interrupt index

#define NULL 			0		
#define FALSE 	0 
#define TRUE 	1
#define MAX_DIGITAL_PINS	48		//digital output

#define DIG_PORT_0	P4	//[00..07]MOLEX_P70/P72 Digital OUT
#define DIG_PORT_1	P5	//[08..15]MOLEX_P71/P73 Digital OUT
#define DIG_PORT_2	P3	//[16..23]DIP SWITCH Digital IN
#define DIG_PORT_3	P2	//[24..31]MOLEX_P69 Digital IN
#define DIG_PORT_4	P6	//[32..39]MOLEX_P63 Digital IN
#define DIG_PORT_5	P7	//[40..47]MOLEX_P68 Digital IN
#define DIG_PORT_x	P0	//[??..??]MOLEX_P68 Digital IN

#define ADDRESS_PORT	P3

// sbit	UART_READ_DISABLE		=	P0^2;				//read enable(active low) max3086 only for GPRIO
// sbit	UART_WRITE_ENABLE		=	P0^3;				//write enable(active high) max3086 only for GPRIO
// sbit	DHT22_LINE				=	P0^3;				//write enable(active high) max3086 only for GPRIO

#define BYTELOW(v)   (*(((unsigned char *) (&v) + 1)))
#define BYTEHIGH(v)  (*((unsigned char *) (&v)))

#define RX_BUFFER_SIZE 16
#define TX_BUFFER_SIZE 16
#define RX_START_FRAME '>'		//pc(tx)->mcu(rx)
#define RX_STOP_FRAME 10		//'\n'
#define TX_START_FRAME '#'		//pc(rx)<-mcu(tx)
#define TX_STOP_FRAME 10		//'\n'
#define RX_ADDRESS_CHAR 1
#define RX_COMMAND_CHAR 3
#define RX_COMMAND_IDX 5
#define RX_COMMAND_VALUE 7

#define RX_COMMAND_SETAOUT_HIGHBYTE 7
#define RX_COMMAND_SETAOUT_LOWBYTE 9
#define RX_COMMAND_GETFLASH_IDX 5
#define RX_COMMAND_SETFLASH_IDX 5
#define RX_COMMAND_SETFLASH_VALUE 7

/*var128 is a 128bytes buffer for flash memory*/
#define	SCRATCH_MEM_START_INDEX	28		
#define	SCRATCH_MEM_END_INDEX	127		
#define	SCRATCH_MEM_INDEX_VIRTUAL_ADDRESS	0		
#define	SCRATCH_MEM_INDEX_SN1 1		
#define	SCRATCH_MEM_INDEX_SN2		2	
#define	SCRATCH_MEM_INDEX_SN3		3
#define	SCRATCH_MEM_INDEX_SN4		4

typedef unsigned char 	u8;
typedef signed   char 	s8;
typedef unsigned int  	u16;
typedef signed   int  	s16;
typedef unsigned long  	u32;
typedef signed   long  	s32;

typedef struct bufType{
	u8 idx;
	u8 size;
	u8 sum;
	u8 buf[32];	
}bufType;

typedef struct msgType{
	u8 xor;
	u8 size;
	u8 addr;
	u8 type;
	u8 idx;
	u8 val;
	u8 t1;
	u8 t2;
}msgType;

typedef struct timeType{
	u16 ms;
	u16 sec;
}timeType;

void Tx_init	();
void Tx_set		(u8 i,u8 var);
void Rx_init	();
void Loopback	();
void SendConstMsg(u16 *pmsg);

void Rxmsg_dout_wr	();
u16 Conv_u8_to_str	(u8 val);
u8 	Conv_str_to_u8	(u16 str2);
u8	Conv_ascii_to_int		(u8 ch);
u8	Conv_int_to_ascii(u8 prm);	//low 4bit hex integer converted to string char, prm=[0x0..0xF]
u8	Rx_get_u8	(u8 i)	;

bit  	get_din	(u8 idx);
u16 	get_ain	(u8 ch,u8 gain);
void 	set_aout(u8 ch, u16 val);
void 	set_dout(u8 idx,u8 val);
void 	set_dout_mode(u8 ch, u8 mode);

void 	ADC0_set_input_pair	(u8 ch,u8 val);

// FLASH read/write/erase routines
void FLASH_ByteWrite (u16 addr, u8 byte, bit SFLE);
u8	 FLASH_ByteRead (u16 addr, bit SFLE);
void FLASH_PageErase (u16 addr, bit SFLE);
void FLASH_Save();
void FLASH_Load();


#endif