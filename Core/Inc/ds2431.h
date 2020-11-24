#ifndef DS2431_H_
#define DS2431_H_

//--------------------------------------------------

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <types.h>

//--------------------------------------------------
//		 ***** MEMORY FUNCTION COMMANDS *****
#define WR_SCRTCH             0x0F     // WRITE SCRATCHPAD 
#define RD_SCRTCH             0xAA     // READ SCRATCHPAD
#define CP_SCRTCH             0x55     // COPY SCRATCHPAD
#define RD_MEM                0xF0     // READ MEMORY
#define WR_APPL_REG           0x99     // WRITE APPLYCATION REGISTER
#define RD_STAT_REG           0x66     // READ STATUS REGISTER
#define RD_APPL_REG           0xC3     // READ APPLYCATION REGISTER
#define CP_LCK_APPL_REG       0x5A     // COPY & LOCK APPLYCATION REGISTER
#define VALID                 0xA5
//		 ***** ROM FUNCTION COMMANDS *****
#define RD_ROM                0x33     // READ ROM
#define MTCH_ROM              0x55     // MATCH ROM
#define SRCH_ROM              0xF0     // SEARCH ROM
#define SKP_ROM               0xCC     // SKIP ROM 
#define WR_SCRTCH             0x0F     // WRITE SCRATCHPAD 
#define RD_SCRTCH             0xAA     // READ SCRATCHPAD
#define CP_SCRTCH             0x55     // COPY SCRATCHPAD
#define RD_MEM                0xF0     // READ MEMORY
#define WR_APPL_REG           0x99     // WRITE APPLYCATION REGISTER
#define RD_STAT_REG           0x66     // READ STATUS REGISTER
#define RD_APPL_REG           0xC3     // READ APPLYCATION REGISTER
#define CP_LCK_APPL_REG       0x5A     // COPY & LOCK APPLYCATION REGISTER
#define VALID                 0xA5
//		 ***** ROM FUNCTION COMMANDS *****
#define RD_ROM                0x33     // READ ROM
#define MTCH_ROM              0x55     // MATCH ROM
#define SRCH_ROM              0xF0     // SEARCH ROM
#define SKP_ROM               0xCC     // SKIP ROM 

# define DS2431_P0_ADDRESS 0x0000 // ?????? ?? 24331 ???????? 0 ????????? ?????
# define DS2431_P1_ADDRESS 0x0020 // ?????? ?? 12431 ???????? 1 ????????? ?????
# define DS2431_P2_ADDRESS 0x0040 // ?????? ?? 24431 ???????? 2 ????????? ?????
# define DS2431_P3_ADDRESS 0x0060 // ?????? ?? 24431 ???????? 3 ????????? ?????
# define DS24311_MAX_ADDRESS 0x007F // ?? 24431 ???????????? ????????? ?????




#define MEMCHIP_SIZE    8   //?????????? ???? ? ?????????? DS2430A

int OwReset (void);
u8 read_2431(void);
u8 new_id(uchar *key);
//bool OwRead_Mem (uchar* str, uchar location, uchar len); 
//bool OwWrite_Mem (uchar* str, uchar location, uchar len);
void Write_Byte(int byte);
int Read_Byte(void);
void Write_Bit(int bit);
void read_2431_pagedata(unsigned char page, unsigned char *page_data);
char write_2431_pagedata(unsigned char page, unsigned char *page_data);

#endif /* DS2431_H_ */