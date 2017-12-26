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
#ifndef URTCLIB
	#define URTCLIB
	#include <Arduino.h>
	#include <Wire.h>
	/*
	RTC I2C Address:
	DS3231 ROM 0x57
	DS3231 RTC 0x68
	*/
	#define URTCLIB_ADDRESS 0x68
	#define URTCLIB_EE_ADDRESS 0x57

	// Convert normal decimal numbers to binary coded decimal
	#define uRTCLIB_decToBcd(val) ((uint8_t) ((val / 10 * 16) + (val % 10)))

	// Convert binary coded decimal to normal decimal numbers
	#define uRTCLIB_bcdToDec(val) ((uint8_t) ((val / 16 * 10) + (val % 16)))


	#ifdef _VARIANT_ARDUINO_STM32_
		#define URTCLIB_INIT_WIRE() if (_do_init) { _do_init = false; Wire.begin(); }
	#else
		#define URTCLIB_INIT_WIRE()
	#endif


	class uRTCLib {
		public:
			// Constructors
			uRTCLib();
			uRTCLib(const int);
			uRTCLib(const int, const int);
			// RTC functions
			void refresh();
			uint8_t second();
			uint8_t minute();
			uint8_t hour();
			uint8_t day();
			uint8_t month();
			uint8_t year();
			uint8_t dayOfWeek();
			void set_rtc_address(const int);
			void set(const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t);
/**
 * Write any datatype from EEPROM address
 *
 * @param unsigned int address Address inside EEPROM to read from
 * @return <typename> read data
 */

	template <typename T> void eeprom_write(const uint16_t address,const T &data) {
		const uint8_t* bytePtrTW = (const uint8_t*)&data;
		Wire.beginTransmission(URTCLIB_EE_ADDRESS);
		Wire.write(address >> 8); // MSB
		Wire.write(address & 0xFF); // LSB
			for (uint8_t i = 0; i < sizeof(T); i++)
			Wire.write(*bytePtrTW++);
		Wire.endTransmission();
	}


/**
 * Read any datatype from EEPROM address
 *
 * @param unsigned int address Address inside EEPROM to read from
 * @return <typename> read data
 */
	template <typename T> void eeprom_read(const uint16_t address, T &data) {
		uint8_t* bytePtrTR      = (uint8_t*)&data;
		Wire.beginTransmission(URTCLIB_EE_ADDRESS);
		Wire.write(address >> 8); // MSB
		Wire.write(address & 0xFF); // LSB
		Wire.endTransmission();
		Wire.requestFrom(URTCLIB_EE_ADDRESS,  sizeof(T));
		for (uint8_t i = 0; i < sizeof(T); i++)
			if (Wire.available()) *bytePtrTR++ = Wire.read();
}

		private:
			// Adresses
			int _rtc_address = URTCLIB_ADDRESS;
			int _ee_address = URTCLIB_EE_ADDRESS;
			// RTC rad data
			uint8_t _second = 0;
			uint8_t _minute = 0;
			uint8_t _hour = 0;
			uint8_t _day = 0;
			uint8_t _month = 0;
			uint8_t _year = 0;
			uint8_t _dayOfWeek = 0;
	
			// STM32 fix - initi aux. flag
			#ifdef _VARIANT_ARDUINO_STM32_
				bool _do_init = true;
			#endif
	};
	

#endif


