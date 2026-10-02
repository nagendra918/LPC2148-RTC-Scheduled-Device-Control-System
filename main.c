//main.c
#include <lpc214x.h>
#include "common/types.h"
#include "common/pin_defines.h"
#include "drivers/delay.h"
#include "drivers/lcd.h"
#include "drivers/kpm.h"
#include "drivers/rtc.h"
#include "drivers/eint.h"
#include "drivers/led.h"

/* GLOBAL VARIABLES */

u32 on_hour = 0;
u32 on_min  = 0;

u32 off_hour = 0;
u32 off_min  = 0;

u32 display_mode = 0;


/* FUNCTION DECLARATIONS */

u32 ReadNumber(u32 digits);

u32 IsLeapYear(u32 year);
u32 DaysInMonth(u32 month, u32 year);

void SetTimeUsingKeypad(void);
void SetDateUsingKeypad(void);
void SetDayUsingKeypad(void);
void SetScheduleUsingKeypad(void);

void DeviceControl(void);
void ConfigMenu(void);

void StartupDisplay(void);
void NormalDisplay(void);

void DisplayDay(u32 day);
void DisplayDeviceStatus(void);
void DisplaySchedule(void);


/* READ NUMBER FROM KEYPAD
   C  -> BACKSPACE
   #  -> ENTER
*/

u32 ReadNumber(u32 digits)
{
    u32 num = 0;
    u32 count = 0;
    u32 key;

    while(1)
    {
        key = KeyScan();

        if(key >= '0' && key <= '9')
        {
            if(count < digits)
            {
                WRITE_LCD_DATA(key);

                num = (num * 10) + (key - '0');
                count++;
            }
        }
        else if(key == 'C')
        {
            if(count > 0)
            {
                count--;

                num = num / 10;

                WRITE_LCD_CMD(0x10);
                WRITE_LCD_DATA(' ');
                WRITE_LCD_CMD(0x10);
            }
        }
        else if(key == '#')
        {
            if(count == digits)
                break;
        }
    }

    return num;
}


/* LEAP YEAR CHECK */

u32 IsLeapYear(u32 year)
{
    if((year % 400) == 0)
        return 1;

    if((year % 100) == 0)
        return 0;

    if((year % 4) == 0)
        return 1;

    return 0;
}


/* DAYS IN MONTH */

u32 DaysInMonth(u32 month, u32 year)
{
    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;

        case 4:
        case 6:
        case 9:
        case 11:
            return 30;

        case 2:
            if(IsLeapYear(year))
                return 29;
            else
                return 28;
    }

    return 0;
}


/* SET TIME */

void SetTimeUsingKeypad(void)
{
    u32 h, m, s;

    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("SET TIME");
    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    strLCD("HH:");
    h = ReadNumber(2);

    WRITE_LCD_DATA(':');

    m = ReadNumber(2);

    WRITE_LCD_DATA(':');

    s = ReadNumber(2);

    if(h < 24 && m < 60 && s < 60)
    {
        SetRTCTimeInfo(h, m, s);

        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("TIME SET");
        delay_s(1);
    }
    else
    {
        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("INVALID TIME");
        delay_s(1);
    }
}


/* SET DATE */

void SetDateUsingKeypad(void)
{
    u32 d, m, y;
    u32 max_days;

    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("SET DATE");
    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    strLCD("DD:");
    d = ReadNumber(2);

    WRITE_LCD_DATA('/');

    m = ReadNumber(2);

    WRITE_LCD_DATA('/');

    y = ReadNumber(4);

    max_days = DaysInMonth(m, y);

    if(m >= 1 && m <= 12 &&
       d >= 1 && d <= max_days)
    {
        SetRTCDateInfo(d, m, y);

        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("DATE SET");
        delay_s(1);
    }
    else
    {
        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("INVALID DATE");
        delay_s(1);
    }
}


/* SET DAY */

void SetDayUsingKeypad(void)
{
    u32 d;

    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("DAY 0-6");
    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    d = ReadNumber(1);

    if(d <= 6)
    {
        SetRTCDay(d);

        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("DAY SET");
        delay_s(1);
    }
    else
    {
        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("INVALID DAY");
        delay_s(1);
    }
}


/* SET ON/OFF SCHEDULE */

void SetScheduleUsingKeypad(void)
{
    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("ON TIME");
    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    strLCD("HH:");
    on_hour = ReadNumber(2);

    WRITE_LCD_DATA(':');

    on_min = ReadNumber(2);

    if(on_hour >= 24 || on_min >= 60)
    {
        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("INVALID ON TIME");
        delay_s(1);
        return;
    }


    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("OFF TIME");
    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    strLCD("HH:");
    off_hour = ReadNumber(2);

    WRITE_LCD_DATA(':');

    off_min = ReadNumber(2);

    if(off_hour >= 24 || off_min >= 60)
    {
        WRITE_LCD_CMD(CLEAR_LCD);
        strLCD("INVALID OFF TIME");
        delay_s(1);
        return;
    }

    WRITE_LCD_CMD(CLEAR_LCD);
    strLCD("SCHEDULE SET");
    delay_s(1);
}


/* DEVICE CONTROL */

void DeviceControl(void)
{
    s32 h, m, s;
    u32 current_time;
    u32 on_time;
    u32 off_time;

    GetRTCTimeInfo(&h, &m, &s);

    current_time = (h * 60) + m;
    on_time      = (on_hour * 60) + on_min;
    off_time     = (off_hour * 60) + off_min;


    /*
       Same ON/OFF time
       Device remains OFF
    */

    if(on_time == off_time)
    {
        LED_Off();
        return;
    }


    /*
       Normal schedule

       Example:
       ON  = 10:00
       OFF = 18:00

       Device ON between 10:00 and 18:00
    */

    if(on_time < off_time)
    {
        if(current_time >= on_time &&
           current_time < off_time)
        {
            LED_On();
        }
        else
        {
            LED_Off();
        }
    }


    /*
       Overnight schedule

       Example:
       ON  = 20:00
       OFF = 06:00

       Device ON:
       20:00 -> 23:59
       00:00 -> 06:00
    */

    else
    {
        if(current_time >= on_time ||
           current_time < off_time)
        {
            LED_On();
        }
        else
        {
            LED_Off();
        }
    }
}


/* CONFIGURATION MENU */

void ConfigMenu(void)
{
    u32 key;

    while(1)
    {
        WRITE_LCD_CMD(CLEAR_LCD);

        strLCD("1.TIME 2.DATE");
        WRITE_LCD_CMD(GOTO_LINE2_POS0);
        strLCD("3.DAY 4.SCH");

        key = KeyScan();

        switch(key)
        {
            case '1':
                SetTimeUsingKeypad();
                break;

            case '2':
                SetDateUsingKeypad();
                break;

            case '3':
                SetDayUsingKeypad();
                break;

            case '4':
                SetScheduleUsingKeypad();
                break;

            case 'D':
                return;
        }
    }
}


/* STARTUP DISPLAY */

void StartupDisplay(void)
{
    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("MOHAMMED SUFIYAN");

    delay_s(1);

    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("Menu-Driven RTC");

    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    strLCD("Configuration");

    delay_s(2);

    WRITE_LCD_CMD(CLEAR_LCD);
}


/* DISPLAY DAY */

void DisplayDay(u32 day)
{
    WRITE_LCD_CMD(GOTO_LINE1_POS0 + 10);

    switch(day)
    {
        case 0:
            strLCD("SUN");
            break;

        case 1:
            strLCD("MON");
            break;

        case 2:
            strLCD("TUE");
            break;

        case 3:
            strLCD("WED");
            break;

        case 4:
            strLCD("THUR");
            break;

        case 5:
            strLCD("FRI");
            break;

        case 6:
            strLCD("SAT");
            break;
    }
}


/* DISPLAY DEVICE STATUS */

void DisplayDeviceStatus(void)
{
    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("DEVICE:");

    if((IOPIN0 >> DEVICE_LED) & 1)
        strLCD("ON");
    else
        strLCD("OFF");
}


/* DISPLAY SCHEDULE */

void DisplaySchedule(void)
{
    WRITE_LCD_CMD(CLEAR_LCD);

    strLCD("ON:");

    WRITE_LCD_DATA(on_hour / 10 + '0');
    WRITE_LCD_DATA(on_hour % 10 + '0');

    WRITE_LCD_DATA(':');

    WRITE_LCD_DATA(on_min / 10 + '0');
    WRITE_LCD_DATA(on_min % 10 + '0');


    WRITE_LCD_CMD(GOTO_LINE2_POS0);

    strLCD("OFF:");

    WRITE_LCD_DATA(off_hour / 10 + '0');
    WRITE_LCD_DATA(off_hour % 10 + '0');

    WRITE_LCD_DATA(':');

    WRITE_LCD_DATA(off_min / 10 + '0');
    WRITE_LCD_DATA(off_min % 10 + '0');
}


/* NORMAL DISPLAY */

void NormalDisplay(void)
{
    s32 h, m, s;
    s32 d, mon, y;
    s32 dow;

    GetRTCTimeInfo(&h, &m, &s);
    GetRTCDateInfo(&d, &mon, &y);
    GetRTCDay(&dow);


    if(display_mode == 0)
    {
        DisplayRTCTime(h, m, s);
        DisplayDay(dow);
    }

    else if(display_mode == 1)
    {
        WRITE_LCD_CMD(CLEAR_LCD);

        WRITE_LCD_CMD(GOTO_LINE1_POS0);

        WRITE_LCD_DATA(d / 10 + '0');
        WRITE_LCD_DATA(d % 10 + '0');

        WRITE_LCD_DATA('/');

        WRITE_LCD_DATA(mon / 10 + '0');
        WRITE_LCD_DATA(mon % 10 + '0');

        WRITE_LCD_DATA('/');

        U32LCD(y);

        DisplayDay(dow);
    }

    else if(display_mode == 2)
    {
        DisplayDeviceStatus();
    }

    else if(display_mode == 3)
    {
        DisplaySchedule();
    }
}


/* MAIN */

int main(void)
{
    u32 key;

    /* Initialize drivers */

    INIT_LCD();
    Init_KPM();
    RTC_Init();
    EINT3_Init();
    LED_Init();


    /* Startup screen */

    StartupDisplay();


    while(1)
    {
        /*
           Check external interrupt request
           for entering configuration mode.
        */

        if(config_request)
        {
            config_request = 0;

            ConfigMenu();

            WRITE_LCD_CMD(CLEAR_LCD);
        }


        /*
           Allow D key to enter configuration menu
           also.
        */

        if(!ColScan())
        {
            key = KeyScan();

            if(key == 'D')
            {
                ConfigMenu();

                WRITE_LCD_CMD(CLEAR_LCD);
            }
        }


        /*
           Automatic device control
           according to schedule.
        */

        DeviceControl();


        /*
           Normal display.
        */

        NormalDisplay();


        /*
           Change display screen.
        */

        delay_s(5);

        display_mode++;

        if(display_mode > 3)
            display_mode = 0;
    }
}
