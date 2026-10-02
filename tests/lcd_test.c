//lcd_test.c
#include "../drivers/lcd.h"
#include "../drivers/delay.h"

int main(void)
{
    INIT_LCD();

    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("LPC2148 LCD");

    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    strLCD("LCD TEST");

    while(1);
}
