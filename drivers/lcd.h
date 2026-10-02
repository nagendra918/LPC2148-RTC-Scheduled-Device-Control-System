//lcd.h
#ifndef LCD_H
#define LCD_H

#include "../common/types.h"

/* LCD Commands */

#define CLEAR_LCD          0x01
#define RET_CUR_HOME       0x02

#define MODE_8BIT_1LINE    0x30
#define MODE_8BIT_2LINE    0x38

#define MODE_4BIT_1LINE    0x20
#define MODE_4BIT_2LINE    0x28

#define DISP_OFF           0x08
#define DISP_ON_CUR_OFF    0x0C
#define DISP_ON_CUR_ON     0x0E
#define DISP_ON_CUR_BLINK  0x0F

#define SHIFT_CUR_RIGHT    0x06

#define GOTO_LINE1_POS0    0x80
#define GOTO_LINE2_POS0    0xC0

#define GOTO_CGRAM         0x40

/* LCD Functions */

void WRITE_LCD_CMD(u8 CMD);
void WRITE_LCD_DATA(u8 ascii);
void INIT_LCD(void);
void strLCD(s8 *str);
void U32LCD(u32 n);

#endif
