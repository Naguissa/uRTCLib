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
	//  RTCLIB_WIRE->begin();
	#endif
	//  refresh();
}


#ifdef _VARIANT_ARDUINO_STM32_
	void RTCLib::_init() {
		RTCLIB_WIRE = new HardWire(RTCLIB_SMT32_HWIREID, I2C_FAST_MODE);
	}
#endif

void RTCLib::refresh() {
	#ifdef _VARIANT_ARDUINO_STM32_
	if(!RTCLIB_WIRE) {
		_init();
	}
	#endif
	RTCLIB_WIRE->beginTransmission(RTCLIB_ADDRESS);
	RTCLIB_WIRE->write(0); // set DS3231 register pointer to 00h
	RTCLIB_WIRE->endTransmission();
	RTCLIB_WIRE->requestFrom(RTCLIB_ADDRESS, 7);
	// request seven uint8_ts of data starting from register 00h
	_second = RTCLIB_WIRE->read();
	_second = RTCLIB_bcdToDec(_second);
	_minute = RTCLIB_WIRE->read();
	_minute = RTCLIB_bcdToDec(_minute);
	_hour = RTCLIB_WIRE->read() & 0b111111;
	_hour = RTCLIB_bcdToDec(_hour);
	_dayOfWeek = RTCLIB_WIRE->read();
	_dayOfWeek = RTCLIB_bcdToDec(_dayOfWeek);
	_day = RTCLIB_WIRE->read();
	_day = RTCLIB_bcdToDec(_day);
	_month = RTCLIB_WIRE->read();
	_month = RTCLIB_bcdToDec(_month);
	_year = RTCLIB_WIRE->read();
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
			if(!RTCLIB_WIRE) {
				_init();
			}
		#endif
		RTCLIB_WIRE->beginTransmission(RTCLIB_ADDRESS);
		RTCLIB_WIRE->write(0); // set next input to start at the seconds register
		RTCLIB_WIRE->write(RTCLIB_decToBcd(second)); // set seconds
		RTCLIB_WIRE->write(RTCLIB_decToBcd(minute)); // set minutes
		RTCLIB_WIRE->write(RTCLIB_decToBcd(hour)); // set hours
		RTCLIB_WIRE->write(RTCLIB_decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
		RTCLIB_WIRE->write(RTCLIB_decToBcd(dayOfMonth)); // set date (1 to 31)
		RTCLIB_WIRE->write(RTCLIB_decToBcd(month)); // set month
		RTCLIB_WIRE->write(RTCLIB_decToBcd(year)); // set year (0 to 99)
		RTCLIB_WIRE->endTransmission();
	}
#endif


#ifdef RTCLIB_EEPROM
	unsigned char RTCLib::eeprom_read(const unsigned int address) {
		unsigned int rdata = 0xFF;
		#ifdef _VARIANT_ARDUINO_STM32_
			if(!RTCLIB_WIRE) {
				_init();
			}
		#endif
		RTCLIB_WIRE->beginTransmission(RTCLIB_EE_ADDRESS);
		RTCLIB_WIRE->write((int)(address >> 8)); // MSB
		RTCLIB_WIRE->write((int)(address & 0xFF)); // LSB
		RTCLIB_WIRE->endTransmission();
		RTCLIB_WIRE->requestFrom(RTCLIB_EE_ADDRESS,1);
		if (RTCLIB_WIRE->available()) {
			rdata = RTCLIB_WIRE->read();
		}
		return rdata;
	}

	void RTCLib::eeprom_write(const unsigned int address, const unsigned char data) {
		#ifdef _VARIANT_ARDUINO_STM32_
			if(!RTCLIB_WIRE) {
				_init();
			}
		#endif
		RTCLIB_WIRE->beginTransmission(RTCLIB_EE_ADDRESS);
		RTCLIB_WIRE->write((int)(address >> 8)); // MSB
		RTCLIB_WIRE->write((int)(address & 0xFF)); // LSB
		RTCLIB_WIRE->write(data);
		delay(5); // Little delay to assure EEPROM is able to process data; if missing and inside for look meses some values
		RTCLIB_WIRE->endTransmission();
	}
#endif

