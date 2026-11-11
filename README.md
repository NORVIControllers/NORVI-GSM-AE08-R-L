# NORVI GSM-AE08-R-L – Hardware Test Example

## Overview

This example demonstrates how to run a full hardware test on the NORVI GSM-AE08-R-L controller.

The purpose of this example is to show how to:

- Start the Wi-Fi access point and print the device MAC address
- Initialize the OLED display, RTC, microSD card and Ethernet
- Test the LTE modem using AT commands
- Send test data over RS-485
- Read the status of the 4 digital inputs and the front-panel button
- Switch the 4 relay outputs ON and OFF in sequence

This example can be used as a starting point for developers building applications with NORVI controllers.

## Product Used

Product: NORVI GSM-AE08-R-L (ESP32-WROOM-32, LTE, Ethernet, RS-485, 4 DI, 4 relay outputs)
Platform: Arduino IDE / PlatformIO (ESP32)

https://norvi.io

## Purpose of This Example

The example demonstrates how to:

- Start a Wi-Fi access point named after the device MAC address
- Read the DS3231 RTC and print the date and time
- Mount the microSD card and print the card type and size
- Start Ethernet (DHCP) and request the time from an NTP server
- Communicate with the LTE modem (EC25 / SIM7500)
- Transmit text over RS-485
- Verify correct operation of the inputs, push button and relays

The program runs the following checks once at start-up, in this order:

- Wi-Fi access point
- RTC
- microSD card
- Ethernet and NTP
- LTE modem (`AT`, `AT+CPIN?`, `AT+GSN`, `ATD+CSQ`)

The program then continuously:

- Prints the state of INPUT 1 to INPUT 4 (0 or 1)
- Prints the push-button analog value
- Switches RELAY 1 → RELAY 2 → RELAY 3 → RELAY 4 ON and OFF, one at a time
- Passes anything typed in the Serial Monitor to the LTE modem and prints the modem reply
- Sends `RS485 01 SUCCESS` over RS-485 and prints any data received

All results are shown on the Serial Monitor.

Follow the steps below to run the example:

### 1. Hardware Connections

- Power the controller with 24 V DC.
- Connect the mini USB cable to your computer.
- Insert a microSD card (optional, for the SD test).
- Connect an Ethernet cable (optional, for the Ethernet test).
- Insert a SIM card and connect the LTE antenna (for the modem test).
- Connect digital input signals to INPUT 1, INPUT 2, INPUT 3 and INPUT 4.
- Connect relay loads (or a multimeter) to the relay outputs.
- For the RS-485 test, connect an RS-485 device or a loopback to the RS-485 terminals.
- Ensure signals are within the supported input range.

### 2. Configure the Program

No additional user configuration is required for this example before upload.

### 3. Upload the Program

- Open the example program in Arduino IDE / PlatformIO
- Select the board **ESP32 Dev Module** and the correct port
- Upload the program to the NORVI controller

### 4. Observe the Serial Monitor

Open the Serial Monitor at **115200 baud**.

When the program runs successfully:

- The Wi-Fi access point starts and the MAC address and AP IP address are printed
- The RTC time is printed
- The SD card is mounted and its type and size are printed
- Ethernet obtains an IP address and prints the NTP time
- The LTE modem replies `OK` to `AT`, then returns the SIM status and IMEI
- The input states are printed continuously
- The relays switch ON and OFF one after another

Example Output:

```
MAC Address: XX:XX:XX:XX:XX:XX
Setting up the Wi-Fi Access Point...
Access Point IP address: 192.168.4.1
Wi-Fi Access Point is active!

2026/3/9 Monday 10:15:32
Card Mount: success
Card Type: SDHC
Unix time = 1789999999
The UTC time is 10:15:32

Testing Modem
AT
OK
+CPIN: READY
OK

1111
Push button  4095

RS485 01 SUCCESS
------------------------------------
```

(Sample values only. The four digits show INPUT 1–4: 1 = OFF, 0 = ON.)

### How to Validate the Example

To verify the example works correctly:

1. Confirm the Wi-Fi access point appears on your phone or computer (SSID = device MAC address, password `12345678`)
2. Confirm the RTC prints a valid date and time
3. Confirm the SD card is mounted with the correct card type and size
4. Confirm Ethernet receives an IP address and the NTP time is printed
5. Confirm the modem returns `OK` for `AT` and a SIM status for `AT+CPIN?`
6. Apply a valid signal to an input and observe the printed value change from 1 to 0
7. Observe each relay click ON and OFF in sequence (or check continuity)
8. Press each front-panel button and confirm the push-button value changes
9. Connect an RS-485 device or loopback and confirm the data is received

### Device Preparation / Configuration

Before running the example ensure:

- The controller is powered correctly
- Required signals are connected
- SIM card and antenna are installed
- Any required configuration is completed

## Required Libraries

Install the following libraries before compiling.

- Wire
- WiFi
- SPI
- FS
- SD
- Adafruit_GFX
- Adafruit_SSD1306
- RTClib
- Ethernet

Installation:

1. Open Arduino IDE
2. Go to Library Manager
3. Search and install the required libraries (Wire, WiFi, SPI, FS and SD are included with the ESP32 core)

## Limitations

- This example is provided for demonstration purposes
- The RTC is set to the compile time if it has lost power
- The OLED is initialized but no custom content is drawn in this example
- The LTE test needs a valid SIM card and network coverage
- The input order follows the test sketch; the datasheet lists DI1–DI4 as GPIO 34, 35, 14, 13, so verify the order on your unit

## Safety Notes

- Do not exceed the rated input voltage
- Do not exceed the relay contact rating
- Ensure proper grounding
- Disconnect power before wiring
- Incorrect wiring may damage the controller

Test Date: 26/10/2026 
Verified By: Kaveesha

## Support

Documentation:
https://norvi.io/docs

User guide: https://norvi.io/docs/norvi-gsm-ae08-r-l-user-guide/ 

For additional support or inquiries, contact the NORVI support team (support@norvi.io).

Datasheet : https://norvi.io/docs/norvi-gsm-ae08-r-l-datasheet/

## License

This example is provided for development and educational purposes.
