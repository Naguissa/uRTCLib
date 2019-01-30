/**
 * DS1307 and DS3231 RTCs basic library
 *
 * Really tiny library to basic RTC functionality on Arduino.
 *
 * DS1307 and DS3231 RTCs are supported. See https://github.com/Naguissa/uEEPROMLib for EEPROM support. Also temperature sensor is supported for DS3231.
 *
 *
 * @copyright Naguissa
 * @author Naguissa
 * @url https://github.com/Naguissa/uRTCLib
 * @url https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html
 * @email naguissa.com@gmail.com
 * @version 5.0.0
 * @created 2015-05-07
 */
#ifndef URTCLIB
	#define URTCLIB
	#include "Arduino.h"
	#include "Wire.h"
	/*
	RTC I2C Address:
	DS3231 RTC 0x68
	*/
	#define URTCLIB_ADDRESS 0x68

	// Convert normal decimal numbers to binary coded decimal
	#define uRTCLIB_decToBcd(val) ((uint8_t) ((val / 10 * 16) + (val % 10)))

	// Convert binary coded decimal to normal decimal numbers
	#define uRTCLIB_bcdToDec(val) ((uint8_t) ((val / 16 * 10) + (val % 16)))

	// ESP8266 yield function
	#if ARDUINO_ARCH_ESP8266
		#define uRTCLIB_YIELD yield();
	#else
		#define uRTCLIB_YIELD
	#endif

	#pragma message "EEPROM functionality moved to https://github.com/Naguissa/uEEPROMLib - Use that library if you receive compiler errors because that."

	class uRTCLib {
		public:
			// Constructors
			uRTCLib();
			uRTCLib(const int);
			uRTCLib(bool);
			uRTCLib(bool, const int);
			// RTC functions
			void refresh();
			uint8_t second();
			uint8_t minute();
			uint8_t hour();
			uint8_t day();
			uint8_t month();
			uint8_t year();
			uint8_t dayOfWeek();
			float temp();
			void set_rtc_address(const int);
			void set(const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t);
		private:
			// Address
			int _rtc_address = URTCLIB_ADDRESS;
			// RTC rad data
			uint8_t _second = 0;
			uint8_t _minute = 0;
			uint8_t _hour = 0;
			uint8_t _day = 0;
			uint8_t _month = 0;
			uint8_t _year = 0;
			uint8_t _dayOfWeek = 0;
			float _temp = 9999;
	};

#endif


