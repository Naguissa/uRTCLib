# Arduino, ESP8266, STM32, ESP32 and others uRTCLib

Master status:   [![Build Status](https://travis-ci.org/Naguissa/uRTCLib.svg?branch=master)](https://travis-ci.org/Naguissa/uRTCLib)

## What is this repository for? ##

Really tiny library to basic RTC functionality on Arduino, ESP8266, STM32, ESP32 and other microcontrollers.

DS1307, DS3231 and DS3232 RTCs are supported (and compatibles)


Supported features:
* SQuare Wave Generator
* Fixed output pin for DS1307
* RAM for DS1307 and DS3232
* temperature sensor for DS3231 and DS3232
* Alarms (1 and 2) for DS3231 and DS3232
* Power lost flag reading and clearing
* Enable Oscillator flag to check if Oscillator will run on VBAT
* Set Clock in 12 hour or 24 hour mode. Get AM PM if in 12 hour mode. (Alarm set still in 24 hour mode)

EEPROM support has been moved to https://github.com/Naguissa/uEEPROMLib



## How do I get set up? ##

You can get it from Arduino libraries directly, searching by uRTCLib.

For manual installation:

 * Get the ZIP.
 * Rename to uRTCLib.zip
 * Install library on Arduino



## Documentation and extras ##

You can find all documentation and extras in this repository: https://github.com/Naguissa/uRTCLib_doc_and_extras

You can read documentation online here: https://naguissa.github.io/uRTCLib_doc_and_extras/



## Examples ##

Included on example folder, available on Arduino IDE.

## Important notes ##

 - Check .h file to see all constants and per-model limitations
 - Alarm pin is normaly HIGH and turns LOW when active.
 - When using alarms, you need to clear the alarm flag manually using alarmClearFlag(). If not done alarm maintains its LOW state.
 - Alarm Flags, A1F and A2F, will be triggered whether or not Alarm Interrupts, A1IE and A2IE, are Enabled.
 - When the RTC register values match alarm register settings, the corresponding Alarm Flag, A1F or A2F, bit is set to logic 1.
 - If using alarmTriggered function to check for alarm trigger, be sure to check alarmMode function to see if alarm is enabled or not.
 - When using alarm interrupts, SQWG is turned off. When using SQWG, alarm interrupts are turned off. They're mutually excluding.



## Who do I talk to? ##

 * [Naguissa](https://github.com/Naguissa)
 * https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html
 * https://www.naguissa.com



## Contribute ##

Any code contribution, report or comment are always welcome. Don't hesitate to use GitHub for that.


 * You can sponsor this project using GitHub's Sponsor button: https://github.com/Naguissa/uRTCLib
 * You can make a donation via PayPal: https://paypal.me/foroelectro


Thanks for your support.


Contributors hall of fame: https://www.foroelectro.net/hall-of-fame-f32/contributors-contribuyentes-t271.html
