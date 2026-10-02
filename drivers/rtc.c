//rtc.c
#include <lpc214x.h>
#include "../common/types.h"
#include "lcd.h"
#include "rtc.h"

#define CPU_LPC2148

/* RTC Variables */

s32 hour, min, sec;
s32 date, month, year;
s32 day;

s8 week[][4] =
{
    "SUN",
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT"
};

/* RTC Initialize */

void RTC_Init(void)
{
#ifdef CPU_LPC2148

    CCR = RTC_ENABLE | RTC_CLKSRC;

#else

    PREINT = PREINT_VAL;
    PREFRAC = PREFRAC_VAL;

    CCR = RTC_ENABLE;

#endif
}

/* Set RTC Time */

void SetRTCTimeInfo(u32 hour, u32 minute, u32 second)
{
    HOUR = hour;
    MIN = minute;
    SEC = second;
}

/* Get RTC Time */

void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
    *hour = HOUR;
    *minute = MIN;
    *second = SEC;
}

/* Display RTC Time */

void DisplayRTCTime(u32 hour, u32 minute, u32 second)
{
    WRITE_LCD_CMD(GOTO_LINE1_POS0);

    WRITE_LCD_DATA(hour / 10 + 48);
    WRITE_LCD_DATA(hour % 10 + 48);

    WRITE_LCD_DATA(':');

    WRITE_LCD_DATA(minute / 10 + 48);
    WRITE_LCD_DATA(minute % 10 + 48);

    WRITE_LCD_DATA(':');

    WRITE_LCD_DATA(second / 10 + 48);
    WRITE_LCD_DATA(second % 10 + 48);
}

/* Set RTC Date */

void SetRTCDateInfo(u32 date, u32 month, u32 year)
{
    DOM = date;
    MONTH = month;
    YEAR = year;
}

/* Get RTC Date */

void GetRTCDateInfo(s32 *date, s32 *month, s32 *year)
{
    *date = DOM;
    *month = MONTH;
    *year = YEAR;
}

/* Display RTC Date */

void DisplayRTCDate(u32 date, u32 month, u32 year)
{
    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    WRITE_LCD_DATA(date / 10 + 48);
    WRITE_LCD_DATA(date % 10 + 48);

    WRITE_LCD_DATA('/');

    WRITE_LCD_DATA(month / 10 + 48);
    WRITE_LCD_DATA(month % 10 + 48);

    WRITE_LCD_DATA('/');

    U32LCD(year);
}

/* Set RTC Day */

void SetRTCDay(u32 dow)
{
    DOW = dow;
}

/* Get RTC Day */

void GetRTCDay(s32 *dow)
{
    *dow = DOW;
}

/* Display RTC Day */

void DisplayRTCDay(u32 day)
{
    WRITE_LCD_CMD(GOTO_LINE1_POS0 + 10);

    strLCD(week[day]);
}
