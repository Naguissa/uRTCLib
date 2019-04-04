/**
 * DS1307, DS3231 and DS3232 RTCs basic library
 *
 * Really tiny library to basic RTC functionality on Arduino.
 *
 * Supported features:
 *     * SQuare Wave Generator
 *     * Fixed output pin for DS1307
 *     * RAM for DS1307 and DS3232
 *     * temperature sensor for DS3231 and DS3232
 *     * Alarms (1 and 2) for DS3231 and DS3232
 *
 * See uEEPROMLib for EEPROM support.
 *
 * @copyright Naguissa
 * @author Naguissa
 * @url https://github.com/Naguissa/uRTCLib
 * @url https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html
 * @email naguissa.com@gmail.com
 * @version 6.0.0
 * @created 2015-05-07
 */
#include "Arduino.h"
#include "Wire.h"
#include "uRTCLib.h"


uRTCLib rtc;

uint8_t position;

void setup() {
delay (2000);
	Serial.begin(9600);
	Serial.println("Serial OK");
	//  Max position: 32767

	#ifdef ARDUINO_ARCH_ESP8266
		Wire.begin(0, 2); // D3 and D4 on ESP8266
	#else
		Wire.begin();
	#endif
	rtc.set_rtc_address(0x68);
	rtc.set_model(URTCLIB_MODEL_DS3232);

	// Only used once, then disabled
	rtc.set(0, 42, 16, 6, 2, 5, 15);
	//  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

	Serial.println("Testing SQWG/INT output:");

	Serial.println("fixed 0:");
	rtc.sqwgSetMode(URTCLIB_SQWG_OFF_0);
	delay(5000);

	Serial.println("fixed 1:");
	rtc.sqwgSetMode(URTCLIB_SQWG_OFF_1);
	delay(5000);

	Serial.println("1 hertz:");
	rtc.sqwgSetMode(URTCLIB_SQWG_1H);
	delay(5000);

	Serial.println("1024 hertz:");
	rtc.sqwgSetMode(URTCLIB_SQWG_1024H);
	delay(5000);

	Serial.println("4096 hertz:");
	rtc.sqwgSetMode(URTCLIB_SQWG_4096H);
	delay(5000);

	Serial.println("8192 hertz:");
	rtc.sqwgSetMode(URTCLIB_SQWG_8192H);
	delay(5000);

	Serial.println("32768 hertz:");
	rtc.sqwgSetMode(URTCLIB_SQWG_32768H);
	delay(5000);


	// Set SRAM (DS3231 doen't have SRAM, so it will not store anything and will return always 0xff)
	for (position = 0; position < 255; position++) {
		rtc.ramWrite(position, position);
	}

	rtc.alarmClearFlag(URTCLIB_ALARM_1);
	rtc.alarmClearFlag(URTCLIB_ALARM_2);

	// Alarm pin will get low at sny :10 seconds. We clear the flag on loop to be able to see it, as it doesn't clear itself when triggered
	rtc.alarmSet(URTCLIB_ALARM_TYPE_1_FIXED_S, 10, 0, 0, 1); // Each minute, at just :10 seconds
	// RTCLib::alarmSet(uint8_t type, uint8_t second, uint8_t minute, uint8_t hour, uint8_t day_dow);

	position = 0;

}

void loop() {
	rtc.refresh();
	Serial.print("RTC DateTime: ");

	Serial.print(rtc.year());
	Serial.print('/');
	Serial.print(rtc.month());
	Serial.print('/');
	Serial.print(rtc.day());

	Serial.print(' ');

	Serial.print(rtc.hour());
	Serial.print(':');
	Serial.print(rtc.minute());
	Serial.print(':');
	Serial.print(rtc.second());

	Serial.print(" DOW: ");
	Serial.print(rtc.dayOfWeek());

	Serial.print(" - Temp: ");
	Serial.print(rtc.temp());

	Serial.print(" - SRAM position ");
	Serial.print(position);
	Serial.print(" value: ");
	Serial.print(rtc.ramRead(position), HEX);

	Serial.println();

	rtc.alarmClearFlag(URTCLIB_ALARM_1);
	rtc.alarmClearFlag(URTCLIB_ALARM_2);


	position++;

	delay(1000);
}
