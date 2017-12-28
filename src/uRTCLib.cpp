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


///**
// * Constructor
// */
//uRTCLib::uRTCLib() {
//	//  refresh();
//}

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


/**
 * Sets RTC EEPROM i2 addres
 *
 * @param int addr RTC i2C address
 */
void uRTCLib::set_ee_address(const uint8_t addr) {
	_ee_address = addr;
}


/**********
 * EEPROM *
 **********/

/**
 * Read one byte
 *
 * @param unsigned int address Address inside EEPROM to read from
 * @return char read byte
 */
uint8_t uRTCLib::_eeprom_read(const unsigned int address) {
	uint8_t rdata = 0xFF;
	Wire.beginTransmission(_ee_address);
	Wire.write((int)(address >> 8)); // MSB
	Wire.write((int)(address & 0xFF)); // LSB
	if (Wire.endTransmission()==0) {
		Wire.requestFrom(_ee_address, 1);
		if (Wire.available()) {
			rdata = (uint8_t) Wire.read();
		}
	}
	return rdata;
}

/**
 * Read sequence of n bytes. Optionally from offset
 *
 * @param unsigned int address Address inside EEPROM to read from
 * @param byte* data Pinter to where read data to
 * @param uint8_t n number of bytes to read
 * @return char read byte
 */
void uRTCLib::eeprom_read(const unsigned int address, uint8_t *data, const uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		*(data + i) = _eeprom_read(address + i);
	}
}


/**
 * Read sequence of n bytes. Optionally from offset
 *
 * @param unsigned int address Address inside EEPROM to read from
 * @param byte* data Pinter to where read data to
 * @param uint8_t n number of bytes to read
 * @param uint8_t offset offset where address start read from
 * @return char read byte
 */
void uRTCLib::eeprom_read(const unsigned int address, uint8_t *data, const uint8_t n, const uint8_t offset) {
	eeprom_read(address, data + offset, n);
}



/**
 * Read a byte from EEPROM address
 *
 * @param unsigned int address Address inside EEPROM to read from
 * @return byte read data
 */
uint8_t uRTCLib::eeprom_read(const unsigned int address) {
	return _eeprom_read(address);
}

/**
 * Write one byte to EEPROM
 *
 * @param unsigned int address Address inside EEPROM to write to
 * @param uint8_t data byte to write
 * @return bool true if successful
 */
bool uRTCLib::_eeprom_write(const unsigned int address, const uint8_t data) {
	Wire.beginTransmission(_ee_address);
	Wire.write((int)(address >> 8)); // MSB
	Wire.write((int)(address & 0xFF)); // LSB
	Wire.write(data);
	delay(5); // Little delay to assure EEPROM is able to process data; if missing and inside for look meses some values
	return Wire.endTransmission() == 0;
}


/**
 * Write sequence of n bytes
 *
 * @param address uint initial addesss to write to
 * @param data *uint8_t pointer to data to write (without offset)
 * @param n uint8_t number of bytes to write
 * @return bool true if successful
 */
bool uRTCLib::eeprom_write(const unsigned int address, const uint8_t *data, const uint8_t n) {
	bool r = true;
	for (uint8_t  i = 0; i < n; i++) {
		r &= _eeprom_write(address + i, (uint8_t) *(data + i));
		if (!r) {
			return false;
		}
	}
	return r;
}


/**
 * Write sequence of n bytes from offset
 *
 * @param address uint initial addesss to write to
 * @param data *uint8_t pointer to data to write (without offset)
 * @param n uint8_t number of bytes to write
 * @param offset uint8_t Offset from address to write to
 * @return bool true if successful
 */
bool uRTCLib::eeprom_write(const unsigned int address, const uint8_t *data, const uint8_t n, const uint8_t offset) {
	return eeprom_write(address + offset, data, n);
}

/**
 * Write one bytes
 *
 * @param address uint initial addesss to write to
 * @param data uint8_t data to write
 * @return bool true if successful
 */
bool uRTCLib::eeprom_write(const unsigned int address, const uint8_t data) {
	return _eeprom_write(address, data);
}

