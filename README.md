# Arduino, ESP8266, STM32, ESP32 and others uRTCLib

## What is this repository for? ##

Really tiny library to basic RTC functionality on Arduino, ESP8266, STM32, ESP32 and other microcontrollers.

DS1307, DS3231 and DS3232 RTCs are supported (and compatibles)


Supported features:
* SQuare Wave Generator
* Fixed output pin for DS1307
* RAM for DS1307 and DS3232
* temperature sensor for DS3231 and DS3232
* Alarms (1 and 2) for DS3231 and DS3232

EEPROM support has been moved to https://github.com/Naguissa/uEEPROMLib



## How do I get set up? ##

You can get it from Arduino libraries directly, searching by uRTCLib.

For manual installation:

 * Get the ZIP.
 * Rename to uRTCLib.zip
 * Install library on Arduino



## Examples ##

Included on example folder, available on Arduino IDE.

## Important notes ##

 - Check .h file to see all constants and per-model limitations
 - Alarm pin is normaly HIGH and turns LOW when active.
 - When using alarms, you need to clear the alarm flag manually using alarmClearFlag(). If not done alarm maintains its LOW state.
 - When using alarms SQWG is turned off. When using SQWG alarms are turned off. They're mutually excluding.



## Who do I talk to? ##

 * [Naguissa](https://github.com/Naguissa)
 * https://www.foroelectro.net/librerias-arduino-ide-f29/ucrc16lib-pequena-libreria-para-calcular-el-ccitt--t214.html
 * https://www.naguissa.com



## Contribute ##

Any code contribution, report or comment are always welcome. Don't hesitate to use GitHub for that.


You can make a donation via PayPal: https://paypal.me/foroelectro


Thanks for your support.


Contributors hall of fame: https://www.foroelectro.net/hall-of-fame-f32/contributors-contribuyentes-t271.html