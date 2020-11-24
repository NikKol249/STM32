#ifndef __DS2430A_UTIL__
#define __DS2430A_UTIL__

#include <stdbool.h>
#include "ds2431.h"

//Глобальный буфер для данных микросхемы DS2430A.
extern u8 memdata [MEMCHIP_SIZE];
//Адрес текущего редактируемого байта.
extern u8 editaddr;

//Флажки для результата возврата функций работы 
// с микросхемой DS2430A.
#define DS2430_OK                0
#define DS2430_NO_IC             (1<<1)
#define DS2430_BAD_READDATA      (1<<2)
#define DS2430_BAD_WRITE         (1<<4)
#define DS2430_BAD_VERIFY        (1<<5)

u8 DS2430Read (u8* buf);
u8 DS2430Write (u8* buf);
u8 DS2430Verify(u8* buf);
void DS2430DumpData (void);
void DS2430movecursor (u8 address);
void DS2430printmembyte (u8 address);
void Write_Byte(int byte);
 int Read_Byte(void);
#endif   //__DS2430A_UTIL__
