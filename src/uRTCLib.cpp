/**
 * DS1307 and DS3231 RTCs with AT24C32 (and compatible) integrated EEPROM basic library
 *
 * Really tiny library to basic RTC and EEPROM (incorporated) functionality on Arduino.
 *
 * DS1307 and DS3231 RTCs are supported AT24C32 EEPROM supported (and compatibles). Also temperature sensor is supported for DS3231.
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
 *
 * @param bool skipInit Set true to skip Wire.init (needed for STM32, SAM and Arduino, at least)
 */
uRTCLib::uRTCLib(bool skipInit) {
	init = skipInit;
}



/**
 * Constructor
 *
 * @param int rtc_address I2C address of RTC
 */
uRTCLib::uRTCLib(const int rtc_address) {
	_rtc_address = rtc_address;
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
}

/**
 * Constructor
 *
 * @param bool skipInit Set true to skip Wire.init (needed for STM32, SAM and Arduino, at least)
 * @param int rtc_address I2C address of RTC
 */
uRTCLib::uRTCLib(bool skipInit, const int rtc_address) {
	init = skipInit;
	_rtc_address = rtc_address;
}

/**
 * Constructor
 *
 * @param bool skipInit Set true to skip Wire.init (needed for STM32, SAM and Arduino, at least)
 * @param int rtc_address I2C address of RTC
 * @param int ee_address I2C address of EEPROM
 */
uRTCLib::uRTCLib(bool skipInit, const int rtc_address, const int ee_address) {
	init = skipInit;
	_rtc_address = rtc_address;
	_ee_address = ee_address;
}

/**
 * Refresh data from HW RTC
 */
void uRTCLib::refresh() {
	uRTCLIB_YIELD
	Wire.beginTransmission(_rtc_address);
	Wire.write(0); // set DS3231 register pointer to 00h
	Wire.endTransmission();
	Wire.requestFrom(_rtc_address, 7);
	// request seven uint8_ts of data starting from register 00h
	_second = Wire.read();
	uRTCLIB_YIELD
	_second = uRTCLIB_bcdToDec(_second);
	_minute = Wire.read();
	uRTCLIB_YIELD
	_minute = uRTCLIB_bcdToDec(_minute);
	_hour = Wire.read() & 0b111111;
	uRTCLIB_YIELD
	_hour = uRTCLIB_bcdToDec(_hour);
	_dayOfWeek = Wire.read();
	uRTCLIB_YIELD
	_dayOfWeek = uRTCLIB_bcdToDec(_dayOfWeek);
	_day = Wire.read();
	uRTCLIB_YIELD
	_day = uRTCLIB_bcdToDec(_day);
	_month = Wire.read();
	uRTCLIB_YIELD
	_month = uRTCLIB_bcdToDec(_month);
	_year = Wire.read();
	uRTCLIB_YIELD
	_year = uRTCLIB_bcdToDec(_year);

	 byte tMSB, tLSB;
     // temp registers (11h-12h) get updated automatically every 64s
     Wire.beginTransmission(_rtc_address);
     Wire.write(0x11);
     Wire.endTransmission();
     Wire.requestFrom(_rtc_address, 2);

     // Should I do more "if available" checks here?
    if(Wire.available()) {
		tMSB = Wire.read(); //2's complement int portion
		tLSB = Wire.read(); //fraction portion
		_temp = ((((short)tMSB << 8) | (short)tLSB) >> 6) / 4.0;
    }
    else {
		_temp = 9999; // Some obvious error value
    }



}


/**
 * Returns actual temperature
 *
 * WARNING: Currently only DS3231 is known to have it at a known address
 *
 * @return float Current stored temperature
 */
float uRTCLib::temp() {

  return _temp;
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
	uRTCLIB_YIELD
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
	uRTCLIB_YIELD
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
byte uRTCLib::_eeprom_read(const unsigned int address) {
	uRTCLIB_STM32_INIT_FIX()
	uRTCLIB_YIELD
	byte rdata = 0xFF;
	Wire.beginTransmission(_ee_address);
	Wire.write((int)(address >> 8)); // MSB
	Wire.write((int)(address & 0xFF)); // LSB
	if (Wire.endTransmission()==0) {
		Wire.requestFrom(_ee_address, 1);
		if(Wire.available()) {
			rdata = (byte) Wire.read();
		}
	}
	uRTCLIB_YIELD
	return rdata;
}

/**
 * Read sequence of n bytes. Optionally from offset
 *
 * @param unsigned int address Address inside EEPROM to read from
 * @param byte* data Pointer to where read data to
 * @param uint8_t n number of bytes to read
 */
void uRTCLib::eeprom_read(const unsigned int address, byte *data, const uint8_t n) {
	unsigned int i;
	for (i = 0; i < n; i++) {
 		byte step = _eeprom_read(address + i);
 		*(data + n - i - 1) = step;
	}
}


/**
 * Read a byte from EEPROM address
 *
 * @param unsigned int address Address inside EEPROM to read from
 * @return byte read data
 */
byte uRTCLib::eeprom_read(const unsigned int address) {
	return _eeprom_read(address);
}

/**
 * Write one byte to EEPROM
 *
 * @param unsigned int address Address inside EEPROM to write to
 * @param byte data byte to write
 * @return bool true if successful
 */
bool uRTCLib::_eeprom_write(const unsigned int address, const byte data) {
	uRTCLIB_YIELD
	Wire.beginTransmission(_ee_address);
	Wire.write((int)(address >> 8)); // MSB
	Wire.write((int)(address & 0xFF)); // LSB
	Wire.write(data);
	uRTCLIB_YIELD
	delay(uRTCLIB_WIRE_DELAY); // Little delay to assure EEPROM is able to process data; if missing and inside for look meses some values
	return Wire.endTransmission() == 0;
}


/**
 * Write sequence of n bytes
 *
 * @param address uint initial addesss to write to
 * @param data *byte pointer to data to write (without offset)
 * @param n uint8_t number of bytes to write
 * @return bool true if successful
 */
bool uRTCLib::eeprom_write(const unsigned int address, void *data, const uint8_t n) {
	bool r = true;
	uint8_t i;
	for (i = 0; i < n; i++) {
		r &= _eeprom_write(address + i, (byte) (((*((uint32_t*) data)) >> ((n - i - 1)*8) & 0xFF)));
	}
	return r;
}


/**
 * Write one byte to EEPROM address
 *
 * Template specialization come to cpp file instead h file
 *
 * @param unsigned int address Address inside EEPROM to write to
 * @param data char data to write
 */
bool uRTCLib::eeprom_write(const unsigned int address, char data) {
	return _eeprom_write(address, data);
}


/**
 * Write one byte to EEPROM address
 *
 * Template specialization come to cpp file instead h file
 *
 * @param unsigned int address Address inside EEPROM to write to
 * @param data unsigned char data to write
 */
bool uRTCLib::eeprom_write(const unsigned int address, unsigned char data) {
	return _eeprom_write(address, data);
}


