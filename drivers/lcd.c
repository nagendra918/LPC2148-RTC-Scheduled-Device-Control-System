//lcd.c
#include <lpc214x.h>

#include "../common/types.h"
#include "../common/defines.h"
#include "../common/pin_defines.h"
#include "delay.h"
#include "lcd.h"

void WRITE_LCD_CMD(u8 CMD)
{
    SCLRBIT(IOCLR0, LCD_RS);
    WRITEBYTE(IOPIN0, LCD_DATA, CMD);
    SSETBIT(IOSET0, LCD_EN);
    delay_us(1);
    SCLRBIT(IOCLR0, LCD_EN);
    delay_ms(2);
}

void WRITE_LCD_DATA(u8 ascii)
{
    SSETBIT(IOSET0, LCD_RS);
    WRITEBYTE(IOPIN0, LCD_DATA, ascii);
    SSETBIT(IOSET0, LCD_EN);
    delay_us(1);
    SCLRBIT(IOCLR0, LCD_EN);
    delay_ms(2);
}

void INIT_LCD(void)
{
    WRITEBYTE(IODIR0, LCD_DATA, 255);
    SETBIT(IODIR0, LCD_RS);
    SETBIT(IODIR0, LCD_EN);
    SCLRBIT(IOCLR0, LCD_RS);
    SCLRBIT(IOCLR0, LCD_EN);
    delay_ms(20);
    WRITE_LCD_CMD(0x30);
    delay_ms(5);
    WRITE_LCD_CMD(0x30);
    delay_us(150);
    WRITE_LCD_CMD(0x30);
    WRITE_LCD_CMD(0x38);
    WRITE_LCD_CMD(0x08);
    WRITE_LCD_CMD(0x01);
    WRITE_LCD_CMD(0x06);
    WRITE_LCD_CMD(0x0C);
}

void strLCD(s8 *str)
{
    while(*str)
    {
        WRITE_LCD_DATA(*str++);
    }
}

void U32LCD(u32 n)
{
    char a[10];
    int i = 0;

    if(n == 0)
    {
        WRITE_LCD_DATA('0');
    }
    else
    {
        while(n)
        {
            a[i++] = n % 10 + '0';
            n = n / 10;
        }

        for(--i; i >= 0; i--)
        {
            WRITE_LCD_DATA(a[i]);
        }
    }
}
