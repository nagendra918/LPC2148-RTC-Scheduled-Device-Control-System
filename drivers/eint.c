//eint.c
#include <lpc214x.h>
#include "../common/types.h"
#include "../common/pin_defines.h"
#include "eint.h"

volatile u32 config_request = 0;

void eint3_isr(void) __irq
{
    config_request = 1;

    EXTINT = (1 << 3);

    VICVectAddr = 0;
}

void EINT3_Init(void)
{
    /* P0.20 -> EINT3 */

    PINSEL1 &= ~(3 << 8);
    PINSEL1 |=  (3 << 8);

    /* Falling edge */

    EXTMODE |= (1 << 3);

    /* Falling edge polarity */

    EXTPOLAR &= ~(1 << 3);

    /* Clear pending EINT3 flag */

    EXTINT = (1 << 3);

    /* Configure VIC */

    VICIntSelect &= ~(1 << EINT3_CHNO);

    VICVectCntl0 = (1 << 5) | EINT3_CHNO;

    VICVectAddr0 = (u32)eint3_isr;

    VICIntEnable = (1 << EINT3_CHNO);
}
