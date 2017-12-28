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
			// EEPROM
			void set_ee_address(const uint8_t);
			// EEPROM read functions
			void eeprom_read(const unsigned int, uint8_t *, const uint8_t);
			void eeprom_read(const unsigned int, uint8_t *, const uint8_t, const uint8_t);
			template <typename TR> void eeprom_read(const unsigned int, TR *);
			uint8_t eeprom_read(const unsigned int);
			// EEPROM write functions
			bool eeprom_write(const unsigned int, const uint8_t *, const uint8_t);
			bool eeprom_write(const unsigned int, const uint8_t *, const uint8_t, const uint8_t);
			bool eeprom_write(const unsigned int, const uint8_t);
 			template <typename TW> bool eeprom_write(const unsigned int, TW);

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
			// EEPROM read and write private functions - works with bytes
			uint8_t _eeprom_read(const unsigned int);
			bool _eeprom_write(const unsigned int, const uint8_t);
	};


	// Templates must be here because Arduino compiler incoptability to declare them on .cpp fil

	/**
	 * Write any datatype to EEPROM address
	 *
	 * @param unsigned int address Address inside EEPROM to write to
	 * @param data <typename> data to write
	 */
	template <typename TW> bool uRTCLib::eeprom_write(const unsigned int address, TW data) {
		return eeprom_write(address, (uint8_t *) &data, (uint8_t) sizeof(TW));
	}


	/**
	 * Read any datatype from EEPROM address
	 *
	 * @param unsigned int address Address inside EEPROM to read from
	 * @return <typename> read data
	 */
	template <typename TR> void uRTCLib::eeprom_read(const unsigned int address, TR *data) {
		eeprom_read(address, (uint8_t *) data, sizeof(TR));
	}

#endif


