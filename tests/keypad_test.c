//keypad_test.c
#include "../drivers/lcd.h"
#include "../drivers/kpm.h"

int main(void)
{
    u32 key;

    INIT_LCD();
    Init_KPM();

    WRITE_LCD_CMD(CLEAR_LCD);
    strLCD("KEY:");

    while(1)
    {
        key = KeyScan();

        WRITE_LCD_CMD(GOTO_LINE1_POS0 + 4);
        WRITE_LCD_DATA(key);
    }
}
