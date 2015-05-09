/**
 * DS1307 and DS3231 RTCs with AT24C32 (and compatible) integrated EEPROM basic library
 *
 * Really tiny library to basic RTC and EEPROM (incorporated) functionality on Arduino.
 *
 * DS1307 and DS3231 RTCs are supported AT24C32 EEPROM supported (and compatibles)
 *
 *
 * @copyright Naguissa
 * @author Naguissa
 * @email naguissa.com@gmail.com
 * @version 1.0
 * @created 2015-05-07
 */
#include <Arduino.h>
#include "RTCLib.h"

RTCLib::RTCLib() {
	#ifndef _VARIANT_ARDUINO_STM32_
	  Wire.begin();
	#endif
	//  refresh();
}


void RTCLib::refresh() {
	#ifdef _VARIANT_ARDUINO_STM32_
		RTCLIB_INIT_WIRE();
	#endif
	Wire.beginTransmission(RTCLIB_ADDRESS);
	Wire.write(0); // set DS3231 register pointer to 00h
	Wire.endTransmission();
	Wire.requestFrom(RTCLIB_ADDRESS, 7);
	// request seven uint8_ts of data starting from register 00h
	_second = Wire.read();
	_second = RTCLIB_bcdToDec(_second);
	_minute = Wire.read();
	_minute = RTCLIB_bcdToDec(_minute);
	_hour = Wire.read() & 0b111111;
	_hour = RTCLIB_bcdToDec(_hour);
	_dayOfWeek = Wire.read();
	_dayOfWeek = RTCLIB_bcdToDec(_dayOfWeek);
	_day = Wire.read();
	_day = RTCLIB_bcdToDec(_day);
	_month = Wire.read();
	_month = RTCLIB_bcdToDec(_month);
	_year = Wire.read();
	_year = RTCLIB_bcdToDec(_year);
}


uint8_t RTCLib::second() {
	return _second;
}

uint8_t RTCLib::minute() {
	return _minute;
}

uint8_t RTCLib::hour() {
	return _hour;
}

uint8_t RTCLib::day() {
	return _day;
}

uint8_t RTCLib::month() {
	return _month;
}

uint8_t RTCLib::year() {
	return _year;
}

uint8_t RTCLib::dayOfWeek() {
	return _dayOfWeek;
}


#ifdef RTCLIB_SET
	void RTCLib::set(const uint8_t second, const uint8_t minute, const uint8_t hour, const uint8_t dayOfWeek, const uint8_t dayOfMonth, const uint8_t month, const uint8_t year) {
		#ifdef _VARIANT_ARDUINO_STM32_
			RTCLIB_INIT_WIRE();
		#endif
		Wire.beginTransmission(RTCLIB_ADDRESS);
		Wire.write(0); // set next input to start at the seconds register
		Wire.write(RTCLIB_decToBcd(second)); // set seconds
		Wire.write(RTCLIB_decToBcd(minute)); // set minutes
		Wire.write(RTCLIB_decToBcd(hour)); // set hours
		Wire.write(RTCLIB_decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
		Wire.write(RTCLIB_decToBcd(dayOfMonth)); // set date (1 to 31)
		Wire.write(RTCLIB_decToBcd(month)); // set month
		Wire.write(RTCLIB_decToBcd(year)); // set year (0 to 99)
		Wire.endTransmission();
	}
#endif


#ifdef RTCLIB_EEPROM
	unsigned char RTCLib::eeprom_read(const unsigned int address) {
		unsigned int rdata = 0xFF;
		#ifdef _VARIANT_ARDUINO_STM32_
			RTCLIB_INIT_WIRE();
		#endif
		Wire.beginTransmission(RTCLIB_EE_ADDRESS);
		Wire.write((int)(address >> 8)); // MSB
		Wire.write((int)(address & 0xFF)); // LSB
		Wire.endTransmission();
		Wire.requestFrom(RTCLIB_EE_ADDRESS,1);
		if (Wire.available()) {
			rdata = Wire.read();
		}
		return rdata;
	}

	void RTCLib::eeprom_write(const unsigned int address, const unsigned char data) {
		#ifdef _VARIANT_ARDUINO_STM32_
			RTCLIB_INIT_WIRE();
		#endif
		Wire.beginTransmission(RTCLIB_EE_ADDRESS);
		Wire.write((int)(address >> 8)); // MSB
		Wire.write((int)(address & 0xFF)); // LSB
		Wire.write(data);
		delay(5); // Little delay to assure EEPROM is able to process data; if missing and inside for look meses some values
		Wire.endTransmission();
	}
#endif

