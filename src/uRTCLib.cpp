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
 * @version 3.2.0
 * @created 2015-05-07
 */
#include <Arduino.h>
#include <Wire.h>
#include "uRTCLib.h"


/**
 * Constructor
 */
uRTCLib::uRTCLib() {
	  
	//  refresh();
}

/**
 * Constructor
 *
 * @param int rtc_address I2C address of RTC
 */
uRTCLib::uRTCLib(const int rtc_address) {
	_rtc_address = rtc_address;
	
	//  refresh();
}

/**
 * Constructor
 *
 * @param int rtc_address I2C address of RTC
 * @param int ee_address I2C address of EEPROM
 */
uRTCLib::uRTCLib(const int rtc_address, const int ee_address) {
	_rtc_address = rtc_address;
	_ee_address = ee_address;
	
	//  refresh();
}

/**
 * Refresh data from HW RTC
 */
void uRTCLib::refresh() {
	URTCLIB_INIT_WIRE()
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

/**
 * Returns actual second
 *
 * @return uint8_t Current stored second
 */
uint8_t uRTCLib::second() {
	return _second;
}

/**
 * Returns actual minute
 *
 * @return uint8_t Current stored minute
 */
uint8_t uRTCLib::minute() {
	return _minute;
}


/**
 * Returns actual hour
 *
 * @return uint8_t Current stored hour
 */
uint8_t uRTCLib::hour() {
	return _hour;
}

/**
 * Returns actual day
 *
 * @return uint8_t Current stored day
 */
uint8_t uRTCLib::day() {
	return _day;
}

/**
 * Returns actual month
 *
 * @return uint8_t Current stored month
 */
uint8_t uRTCLib::month() {
	return _month;
}

/**
 * Returns actual year
 *
 * @return uint8_t Current stored year
 */
uint8_t uRTCLib::year() {
	return _year;
}

/**
 * Returns actual Day Of Week
 *
 * @return uint8_t Current stored Day Of Week
 */
uint8_t uRTCLib::dayOfWeek() {
	return _dayOfWeek;
}

/**
 * Sets RTC i2 addres
 *
 * @param int RTC i2C address
 */
void uRTCLib::set_rtc_address(const int addr) {
	_rtc_address = addr;
}


/**
 * Sets RTC datetime data
 *
 * @param uint8_t second second to set to HW RTC
 * @param uint8_t minute minute to set to HW RTC
 * @param uint8_t hour hour to set to HW RTC
 * @param uint8_t dayOfWeek day of week to set to HW RTC
 * @param uint8_t dayOfMonth day of month to set to HW RTC
 * @param uint8_t month month to set to HW RTC
 * @param uint8_t year year to set to HW RTC
 */
void uRTCLib::set(const uint8_t second, const uint8_t minute, const uint8_t hour, const uint8_t dayOfWeek, const uint8_t dayOfMonth, const uint8_t month, const uint8_t year) {
	URTCLIB_INIT_WIRE()
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


