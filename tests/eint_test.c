//eint_test.c
#include "../drivers/eint.h"
#include "../drivers/led.h"
#include "../drivers/delay.h"

int main(void)
{
    LED_Init();
    EINT3_Init();

    while(1)
    {
        if(config_request)
        {
            config_request = 0;

            LED_On();
            delay_s(1);
            LED_Off();
        }
    }
}