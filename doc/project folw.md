# Project Flow

## 1. Overall System Flow

```text
                         START
                           |
                           v
                  Initialize LCD
                           |
                           v
                Initialize Keypad
                           |
                           v
                   Initialize RTC
                           |
                           v
                  Initialize EINT3
                           |
                           v
                    Initialize LED
                           |
                           v
                   Startup Display
                           |
                           v
                      Main Loop
                           |
              +------------+------------+
              |                         |
              v                         v
       Check Config Request       Control Device
              |                    Using Schedule
              v                         |
       Request Present?                 |
          /       \                     |
        YES        NO                   |
         |          |                   |
         v          |                   |
   Configuration   |                    |
       Menu        |                    |
         |          |                   |
         v          |                   |
   +-----+-----+    |                   |
   |     |     |    |                   |
 Time  Date   Day  Schedule              |
   |     |     |    |                   |
   +-----+-----+----+-------------------+
                     |
                     v
               Normal Display
                     |
                     v
                 Repeat Loop
```

---

## 2. Configuration Menu Flow

```text
              EINT3 / D Key
                    |
                    v
            Configuration Menu
                    |
       +------------+------------+
       |            |            |
       v            v            v
    1. Time      2. Date      3. Day
       |            |            |
       +------------+------------+
                    |
                    v
               4. Schedule
                    |
                    v
              Return to Main
```

---

## 3. Time Configuration

```text
           Select Time
                |
                v
          Enter HH:MM:SS
                |
                v
             Validate
                |
          +-----+-----+
          |           |
        Valid       Invalid
          |           |
          v           v
       Set RTC    Show Error
          |           |
          +-----+-----+
                |
                v
          Return to Main
```

---

## 4. Date Configuration

```text
           Select Date
                |
                v
         Enter DD/MM/YYYY
                |
                v
       Validate Date
       + Check Leap Year
                |
          +-----+-----+
          |           |
        Valid       Invalid
          |           |
          v           v
      Set RTC Date  Show Error
          |           |
          +-----+-----+
                |
                v
          Return to Main
```

---

## 5. Day Configuration

```text
            Select Day
                |
                v
             Enter Day
                |
                v
              Validate
                |
          +-----+-----+
          |           |
        Valid       Invalid
          |           |
          v           v
       Set RTC Day  Show Error
          |           |
          +-----+-----+
                |
                v
          Return to Main
```

---

## 6. Schedule Configuration

```text
          Select Schedule
                |
                v
           Enter ON Time
                |
                v
          Enter OFF Time
                |
                v
             Validate
                |
          +-----+-----+
          |           |
        Valid       Invalid
          |           |
          v           v
    Compare ON/OFF   Show Error
        Times
          |
      +---+---+
      |       |
     Same  Different
      |       |
      v       v
    Reject   Store
   Schedule Schedule
      |       |
      +---+---+
          |
          v
     Return to Main
```

---

## 7. Device Control Flow

```text
             Read RTC Time
                   |
                   v
        Calculate Current Time
                   |
                   v
        Compare With Schedule
                   |
             +-----+-----+
             |           |
          Within       Outside
         Schedule      Schedule
             |           |
             v           v
         Device ON    Device OFF
             |           |
             +-----+-----+
                   |
                   v
               Repeat
```

---

## 8. LCD Display Flow

```text
            Current Time
                  |
               5 sec
                  v
            Current Date
                  |
               5 sec
                  v
             Device Status
                  |
               5 sec
                  v
           ON/OFF Schedule
                  |
               5 sec
                  |
                  +--------> Repeat
```

---

## 9. Project Architecture

```text
                       main.c
                         |
          +--------------+--------------+
          |              |              |
          v              v              v
        LCD           Keypad           RTC
          |              |              |
          +--------------+--------------+
                         |
              +----------+----------+
              |                     |
              v                     v
            EINT3                  LED
              |
              v
       Configuration Request
```

**Application:** `main.c`
**Hardware Drivers:** LCD, Keypad, RTC, EINT3, LED, Delay
**Common:** Types, macros, pin definitions
