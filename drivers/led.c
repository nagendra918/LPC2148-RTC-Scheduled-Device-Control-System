//led.c
#include <lpc214x.h>
#include "../common/defines.h"
#include "../common/pin_defines.h"
#include "led.h"

void LED_Init(void)
{
    SETBIT(IODIR0, DEVICE_LED);
    SETBIT(IOCLR0, DEVICE_LED);
}

void LED_On(void)
{
    SETBIT(IOSET0, DEVICE_LED);
}

void LED_Off(void)
{
    SETBIT(IOCLR0, DEVICE_LED);
}
