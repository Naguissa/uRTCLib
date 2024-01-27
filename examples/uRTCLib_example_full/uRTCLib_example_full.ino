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
 *     * Power lost flag reading and clearing
 *     * Enable Oscillator flag to check if Oscillator will run on VBAT
 *     * Set Clock in 12 hour or 24 hour mode. Get AM PM if in 12 hour mode. (Alarm set still in 24 hour mode)
 *
 * See uEEPROMLib for EEPROM support.
 *
 * @copyright Naguissa
 * @author Naguissa
 * @url https://github.com/Naguissa/uRTCLib
 * @url https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html
 * @email naguissa@foroelectro.net
 */
#include "Arduino.h"
#include "uRTCLib.h"


uRTCLib rtc;

// URTCLIB_MODEL_DS1307
// URTCLIB_MODEL_DS3231
// URTCLIB_MODEL_DS3232
byte rtcModel = URTCLIB_MODEL_DS3231;

uint8_t position;

void setup() {
	delay (2000);
	Serial.begin(9600);
	Serial.println("Serial OK");
	//  Max position: 32767

	#ifdef ARDUINO_ARCH_ESP8266
		URTCLIB_WIRE.begin(0, 2); // D3 and D4 on ESP8266
	#else
		URTCLIB_WIRE.begin();
	#endif
	rtc.set_rtc_address(0x68);

	// set RTC Model
	rtc.set_model(rtcModel);

	// refresh data from RTC HW in RTC class object so flags like rtc.lostPower(), rtc.getEOSCFlag(), etc, can get populated
	rtc.refresh();

	// Only use once, then disable
	// rtc.set(0, 42, 16, 6, 2, 5, 15);
	//  RTCLib::set(byte second, byte minute, byte hour (0-23:24-hr mode only), byte dayOfWeek (Sun = 1, Sat = 7), byte dayOfMonth (1-12), byte month, byte year)

	// use the following if you want to set main clock in 12 hour mode
	// rtc.set_12hour_mode(true);

	// use the following if you want to set main clock in 24 hour mode
	// rtc.set_12hour_mode(false);

	if (rtc.enableBattery()) {
		Serial.println("Battery activated correctly.");
	} else {
		Serial.println("ERROR activating battery.");
	}

	// Check whether OSC is set to use VBAT or not
	if (rtc.getEOSCFlag())
		Serial.println(F("Oscillator will not use VBAT when VCC cuts off. Time will not increment without VCC!"));
	else
		Serial.println(F("Oscillator will use VBAT when VCC cuts off."));

	Serial.print("Lost power status: ");
	if (rtc.lostPower()) {
		Serial.print("POWER FAILED. Clearing flag...");
		rtc.lostPowerClear();
		Serial.println(" done.");
	} else {
		Serial.println("POWER OK");
	}


	Serial.print("Aging register value: ");
	Serial.println(rtc.agingGet());

	Serial.println("Not changing aging register value. To do so you can execute: rtc.agingSet(newValue)");


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

	if(rtcModel == URTCLIB_MODEL_DS3231 || rtcModel == URTCLIB_MODEL_DS3232) {
		rtc.alarmClearFlag(URTCLIB_ALARM_1);
		rtc.alarmClearFlag(URTCLIB_ALARM_2);

		// Alarm pin will get low at sny :10 seconds. We clear the flag on loop to be able to see it, as it doesn't clear itself when triggered
		rtc.alarmSet(URTCLIB_ALARM_TYPE_1_FIXED_S, 10, 0, 0, 1); // Each minute, at just :10 seconds
		// RTCLib::alarmSet(uint8_t type, uint8_t second, uint8_t minute, uint8_t hour, uint8_t day_dow);
	}

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
	if(rtc.hourModeAndAmPm() == 1)
		Serial.print(" AM");
	if(rtc.hourModeAndAmPm() == 2)
		Serial.print(" PM");

	Serial.print(" DOW: ");
	Serial.print(rtc.dayOfWeek());

	Serial.print(" - Temp: ");
	Serial.print((float) rtc.temp() / 100);

	if(rtcModel == URTCLIB_MODEL_DS1307 || rtcModel == URTCLIB_MODEL_DS3232) {
		Serial.print(" - SRAM position ");
		Serial.print(position);
		Serial.print(" value: ");
		Serial.print(rtc.ramRead(position), HEX);
		position++;
	}

	Serial.println();

	if(rtcModel == URTCLIB_MODEL_DS3231 || rtcModel == URTCLIB_MODEL_DS3232) {
		rtc.alarmClearFlag(URTCLIB_ALARM_1);
		rtc.alarmClearFlag(URTCLIB_ALARM_2);
	}

	delay(1000);
}
