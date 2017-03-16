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
#ifndef URTCLIB
	#define URTCLIB
	#include "Arduino.h"
	#include "Wire.h"
	/*
	RTC I2C Address:
	DS3231 ROM 0x57
	DS3231 RTC 0x68
	*/
	#define URTCLIB_ADDRESS 0x68
	#define URTCLIB_EE_ADDRESS 0x57

	//Comment to disable RTC-setting function
	#define URTCLIB_SET

	//Comment to disable EEPROM functionality
	#define URTCLIB_EEPROM

	// Convert normal decimal numbers to binary coded decimal
	#define uRTCLIB_decToBcd(val) ((uint8_t) ((val / 10 * 16) + (val % 10)))
	//#define RTCLIB_decToBcd(val) ((uint8_t) (val + 6 * (val / 10)))

	// Convert binary coded decimal to normal decimal numbers
	//#define RTCLIB_bcdToDec(val) ((uint8_t) (val - 6 * (val >> 4)))
	#define uRTCLIB_bcdToDec(val) ((uint8_t) ((val / 16 * 10) + (val % 16)))


	#ifdef _VARIANT_ARDUINO_STM32_
		#define URTCLIB_INIT_WIRE() if (_do_init) { _do_init = false; Wire.begin(); }
	#endif


	class uRTCLib {
		public:
			uRTCLib();
			uint8_t second();
			uint8_t minute();
			uint8_t hour();
			uint8_t day();
			uint8_t month();
			uint8_t year();
			uint8_t seconds();
			uint8_t dayOfWeek();
			void refresh();
			void set_rtc_address(int);

			#ifdef URTCLIB_SET
				void set(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year);
			#endif

			#ifdef URTCLIB_EEPROM
				void set_ee_address(int);
				unsigned char eeprom_read(const unsigned int address);
				void eeprom_write(const unsigned int address, const unsigned char data);
			#endif

		private:
			int _rtc_address = URTCLIB_ADDRESS;
			#ifdef URTCLIB_EEPROM
				int _ee_address = URTCLIB_EE_ADDRESS;
			#endif
		
			uint8_t _second = 0;
			uint8_t _minute = 0;
			uint8_t _hour = 0;
			uint8_t _day = 0;
			uint8_t _month = 0;
			uint8_t _year = 0;
			uint8_t _dayOfWeek = 0;
			#ifdef _VARIANT_ARDUINO_STM32_
				bool _do_init = true;
			#endif
	};
#endif
