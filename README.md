# Menu-Driven RTC Configuration and Scheduled Device Control System

A menu-driven embedded system developed using the **LPC2148 ARM7 microcontroller** to configure RTC time, date, day, and device ON/OFF schedules through a **4x4 keypad**, with information displayed on a **16x2 LCD**.

The system automatically controls a device according to the configured RTC schedule and provides configuration access through an **EINT3 external interrupt switch**.

---

## Features

* RTC-based real-time clock display
* Configure:

  * Time
  * Date
  * Day
  * Device ON/OFF schedule
* 4x4 keypad for user input
* 16x2 LCD for menu and status display
* External interrupt (EINT3) for configuration mode
* Automatic device ON/OFF control based on schedule
* Supports schedules crossing midnight
* Leap-year validation for date configuration
* Modular hardware driver structure
* Separate hardware testing files

---

## Hardware Requirements

* LPC2148 ARM7 Development Board
* 16x2 LCD
* 4x4 Matrix Keypad
* LED / Device
* External switch for EINT3
* Suitable power supply

---

## Pin Configuration

| Peripheral     | LPC2148 Pin   |
| -------------- | ------------- |
| LCD Data D0-D7 | P0.8 - P0.15  |
| LCD RS         | P0.16         |
| LCD EN         | P0.18         |
| Keypad Rows    | P1.16 - P1.19 |
| Keypad Columns | P1.20 - P1.23 |
| Device LED     | P0.7          |
| EINT3          | P0.20         |

RTC uses the internal RTC peripheral of the LPC2148 and does not require external GPIO pins.

---

## Project Structure

```text
Menu-Driven-RTC-Configuration-and-Scheduled-Device-Control-System/
│
├── common/
│   ├── defines.h
│   ├── pin_defines.h
│   └── types.h
│
├── drivers/
│   ├── delay.c
│   ├── delay.h
│   ├── eint.c
│   ├── eint.h
│   ├── kpm.c
│   ├── kpm.h
│   ├── lcd.c
│   ├── lcd.h
│   ├── led.c
│   ├── led.h
│   ├── rtc.c
│   └── rtc.h
│
├── tests/
│   ├── eint_test.c
│   ├── keypad_test.c
│   ├── lcd_test.c
│   ├── led_test.c
│   └── rtc_test.c
│
├── doc/
│   ├── PROJECT_FLOW.md
│   └── blok diagram.png
│
├── images/
│   └── ...
│
├── main.c
├── README.md
└── .gitignore
```

---

## System Flow

```text
                         START
                           |
                           v
                  Initialize Drivers
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
       Configuration Menu               |
              |                         |
       +------+------+------+           |
       |      |      |      |           |
      Time   Date   Day  Schedule       |
       |      |      |      |           |
       +------+------+------+------------+
                           |
                           v
                     LCD Display
                           |
                           v
                      Repeat Loop
```

For detailed flow, see **[doc/PROJECT_FLOW.md](doc/PROJECT_FLOW.md)**.

---

## Configuration Menu

Configuration mode can be entered using the **EINT3 switch** or the `D` key.

```text
+----------------+
| 1.TIME  2.DATE |
| 3.DAY   4.SCH  |
+----------------+
```

### 1. Time

Enter:

```text
HH:MM:SS
```

The entered values are validated before updating the RTC.

### 2. Date

Enter:

```text
DD/MM/YYYY
```

The system validates the month, number of days, and leap years.

### 3. Day

The day is selected using:

```text
0 -> SUN
1 -> MON
2 -> TUE
3 -> WED
4 -> THUR
5 -> FRI
6 -> SAT
```

### 4. Schedule

Configure:

```text
ON  TIME
OFF TIME
```

Example:

```text
ON  = 10:00
OFF = 18:00
```

The device automatically turns ON between the configured times.

---

## Overnight Scheduling

The system also supports schedules that cross midnight.

Example:

```text
ON  = 20:00
OFF = 06:00
```

The device remains ON from:

```text
20:00 -> 23:59
00:00 -> 06:00
```

and remains OFF during the remaining time.

---

## LCD Display

The LCD cycles through system information at 5-second intervals.

```text
Current Time
      ↓
Current Date
      ↓
Device Status
      ↓
ON/OFF Schedule
      ↓
Repeat
```

---

## Keypad Controls

| Key   | Function                  |
| ----- | ------------------------- |
| `0-9` | Numeric input             |
| `C`   | Backspace                 |
| `#`   | Confirm input             |
| `D`   | Configuration menu / exit |

---

## Software

* **Microcontroller:** LPC2148 ARM7
* **Programming Language:** Embedded C
* **IDE:** Keil µVision
* **Version Control:** Git / GitHub

---

## Drivers

The project uses separate drivers for hardware modules:

| Driver | Responsibility                             |
| ------ | ------------------------------------------ |
| Delay  | Microsecond, millisecond and second delays |
| LCD    | LCD initialization and display operations  |
| Keypad | Key scanning and numeric input             |
| RTC    | Time, date and day management              |
| EINT3  | External interrupt configuration           |
| LED    | Device ON/OFF control                      |

The application logic remains in a **single `main.c`** file.

---

## Testing

Each hardware driver has an independent test file in the `tests/` directory.

Drivers can be tested individually on the hardware before integrating them with the main application.

> Only one test file containing `main()` should be added to the Keil target at a time.

---

## Main Application

The application performs:

1. Driver initialization
2. Startup display
3. Configuration request checking
4. RTC configuration
5. Schedule configuration
6. Automatic device control
7. LCD status display
8. Continuous system operation

---

## Future Improvements

* Password protection for configuration mode
* Configuration menu timeout
* Timer-based menu timeout instead of blocking delays
* Additional controlled devices
* EEPROM-based storage for schedule settings

---

## Author

**Nagendra Babu**

B.Tech – Electronics and Communication Engineering

