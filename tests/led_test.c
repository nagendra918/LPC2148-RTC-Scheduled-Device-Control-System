//led_test.c
#include "../drivers/led.h"
#include "../drivers/delay.h"

int main(void)
{
    LED_Init();

    while(1)
    {
        LED_On();
        delay_s(1);

        LED_Off();
        delay_s(1);
    }
}
