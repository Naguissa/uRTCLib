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
#include "uRTCLib.h"

uRTCLib::uRTCLib() {
	#ifndef _VARIANT_ARDUINO_STM32_
	  Wire.begin();
	#endif
	//  refresh();
}


void uRTCLib::refresh() {
	#ifdef _VARIANT_ARDUINO_STM32_
		URTCLIB_INIT_WIRE();
	#endif
	Wire.beginTransmission(_rtc_address);
	Wire.write(0); // set DS3231 register pointer to 00h
	Wire.endTransmission();
	Wire.requestFrom(_rtc_address, 7);
	// request seven uint8_ts of data starting from register 00h
	_second = Wire.read();
	_second = uRTCLIB_bcdToDec(_second);
	_minute = Wire.read();
	_minute = uRTCLIB_bcdToDec(_minute);
	_hour = Wire.read() & 0b111111;
	_hour = uRTCLIB_bcdToDec(_hour);
	_dayOfWeek = Wire.read();
	_dayOfWeek = uRTCLIB_bcdToDec(_dayOfWeek);
	_day = Wire.read();
	_day = uRTCLIB_bcdToDec(_day);
	_month = Wire.read();
	_month = uRTCLIB_bcdToDec(_month);
	_year = Wire.read();
	_year = uRTCLIB_bcdToDec(_year);
}


uint8_t uRTCLib::second() {
	return _second;
}

uint8_t uRTCLib::minute() {
	return _minute;
}

uint8_t uRTCLib::hour() {
	return _hour;
}

uint8_t uRTCLib::day() {
	return _day;
}

uint8_t uRTCLib::month() {
	return _month;
}

uint8_t uRTCLib::year() {
	return _year;
}

uint8_t uRTCLib::dayOfWeek() {
	return _dayOfWeek;
}

void uRTCLib::set_rtc_address(int addr) {
	_rtc_address = addr;
}


#ifdef URTCLIB_SET
	void uRTCLib::set(const uint8_t second, const uint8_t minute, const uint8_t hour, const uint8_t dayOfWeek, const uint8_t dayOfMonth, const uint8_t month, const uint8_t year) {
		#ifdef _VARIANT_ARDUINO_STM32_
			URTCLIB_INIT_WIRE();
		#endif
		Wire.beginTransmission(_rtc_address);
		Wire.write(0); // set next input to start at the seconds register
		Wire.write(uRTCLIB_decToBcd(second)); // set seconds
		Wire.write(uRTCLIB_decToBcd(minute)); // set minutes
		Wire.write(uRTCLIB_decToBcd(hour)); // set hours
		Wire.write(uRTCLIB_decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
		Wire.write(uRTCLIB_decToBcd(dayOfMonth)); // set date (1 to 31)
		Wire.write(uRTCLIB_decToBcd(month)); // set month
		Wire.write(uRTCLIB_decToBcd(year)); // set year (0 to 99)
		Wire.endTransmission();
	}
#endif


#ifdef URTCLIB_EEPROM
	void uRTCLib::set_ee_address(int addr) {
		_ee_address = addr;
	}

	unsigned char uRTCLib::eeprom_read(const unsigned int address) {
		unsigned int rdata = 0xFF;
		#ifdef _VARIANT_ARDUINO_STM32_
			URTCLIB_INIT_WIRE();
		#endif
		Wire.beginTransmission(_ee_address);
		Wire.write((int)(address >> 8)); // MSB
		Wire.write((int)(address & 0xFF)); // LSB
		Wire.endTransmission();
		Wire.requestFrom(_ee_address, 1);
		if (Wire.available()) {
			rdata = Wire.read();
		}
		return rdata;
	}

	void uRTCLib::eeprom_write(const unsigned int address, const unsigned char data) {
		#ifdef _VARIANT_ARDUINO_STM32_
			URTCLIB_INIT_WIRE();
		#endif
		Wire.beginTransmission(_ee_address);
		Wire.write((int)(address >> 8)); // MSB
		Wire.write((int)(address & 0xFF)); // LSB
		Wire.write(data);
		delay(5); // Little delay to assure EEPROM is able to process data; if missing and inside for look meses some values
		Wire.endTransmission();
	}
#endif

