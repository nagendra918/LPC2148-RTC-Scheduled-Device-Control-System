#include "../drivers/lcd.h"
#include "../drivers/delay.h"
#include "../drivers/led.h"
int main(void)
{
	LED_Init();
	/*
    INIT_LCD();

    WRITE_LCD_CMD(GOTO_LINE1_POS0);
    strLCD("LPC2148 LCD");

    WRITE_LCD_CMD(GOTO_LINE2_POS0);
    strLCD("LCD TEST");
	*/

		
    while(1)
    {
        LED_On();
        delay_s(1);

        LED_Off();
        delay_s(1);
    }
}