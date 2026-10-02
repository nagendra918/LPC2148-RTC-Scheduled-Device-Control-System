//rtc_test.c
#include "../drivers/lcd.h"
#include "../drivers/rtc.h"
#include "../drivers/delay.h"

int main(void)
{
    s32 h, m, s;
    s32 d, mon, y;
    s32 dow;

    INIT_LCD();
    RTC_Init();

    SetRTCTimeInfo(10, 30, 00);
    SetRTCDateInfo(2, 10, 2026);
    SetRTCDay(5);

    while(1)
    {
        GetRTCTimeInfo(&h, &m, &s);
        GetRTCDateInfo(&d, &mon, &y);
        GetRTCDay(&dow);

        DisplayRTCTime(h, m, s);
        DisplayRTCDay(dow);

        delay_s(1);
    }
}