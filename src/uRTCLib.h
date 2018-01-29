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

	// Wire delay
	#define uRTCLIB_WIRE_DELAY 5



	class uRTCLib {
		public:
			// Constructors
			uRTCLib();
			uRTCLib(const int);
			uRTCLib(const int, const int);
			uRTCLib(bool);
			uRTCLib(bool, const int);
			uRTCLib(bool, const int, const int);
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
			// EEPROM
			void set_ee_address(const uint8_t);
			// EEPROM read functions
			void eeprom_read(const unsigned int, byte *, const uint8_t);
			template <typename TR> void eeprom_read(const unsigned int, TR *);
			byte eeprom_read(const unsigned int);
			// EEPROM write functions
			bool eeprom_write(const unsigned int, void *, const uint8_t);
			bool eeprom_write(const unsigned int, char);
			bool eeprom_write(const unsigned int, unsigned char);
 			template <typename TW> bool eeprom_write(const unsigned int, const TW);
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
			float _temp = 9999;
			// EEPROM read and write private functions - works with bytes
			byte _eeprom_read(const unsigned int);
			bool _eeprom_write(const unsigned int, const byte);
			// Fix for 1st write error
			bool init = false;
			#define uRTCLIB_STM32_INIT_FIX() { if (!init) { init = true;  _eeprom_read(0); delay(10); } }
	};


	// Templates must be here because Arduino compiler incoptability to declare them on .cpp fil

	/**
	 * Write any datatype to EEPROM address
	 *
	 * @param unsigned int address Address inside EEPROM to write to
	 * @param data <typename> data to write
	 */
	template <typename TW> bool uRTCLib::eeprom_write(const unsigned int address, const TW data) {
		return eeprom_write(address, (void *) &data, sizeof(TW));
	}


	/**
	 * Read any datatype from EEPROM address
	 *
	 * @param unsigned int address Address inside EEPROM to read from
	 * @return <typename> read data
	 */
	template <typename TR> void uRTCLib::eeprom_read(const unsigned int address, TR *data) {
		eeprom_read(address, (byte *) data, sizeof(TR));
	}
#endif


