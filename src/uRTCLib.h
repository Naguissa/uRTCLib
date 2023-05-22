/**
 * \mainpage
 * \brief DS1307, DS3231 and DS3232 RTCs basic library
 *
 * Really tiny library to basic RTC functionality on Arduino.
 *
 * Supported features:
 *     * SQuare Wave Generator
 *     * Fixed output pin for DS1307
 *     * RAM for DS1307 and DS3232
 *     * temperature sensor for DS3231 and DS3232
 *     * Alarms (1 and 2) for DS3231 and DS3232
 *     * Power failure check and clear
 *
 * See uEEPROMLib for EEPROM support, https://github.com/Naguissa/uEEPROMLib
 *
 * Note: For AtTiny you need TinyWireM library from Adafruit installed (available on library manager).
 *
 *
 * @see <a href="https://github.com/Naguissa/uRTCLib">https://github.com/Naguissa/uRTCLib</a>
 * @see <a href="https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html">https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 * @see <a href="https://github.com/Naguissa/uEEPROMLib">See uEEPROMLib for EEPROM support.</a>
 * @version 6.6.1
 */
/** \file uRTCLib.h
 *   \brief uRTCLib header file
 */
#ifndef URTCLIB
	/**
	 * \brief Prevent multiple inclussion
	 */
	#define URTCLIB
	#include "Arduino.h"
	#ifndef URTCLIB_WIRE
		#if defined(ARDUINO_attiny) || defined(ARDUINO_AVR_ATTINYX4) || defined(ARDUINO_AVR_ATTINYX5) || defined(ARDUINO_AVR_ATTINYX7) || defined(ARDUINO_AVR_ATTINYX8) || defined(ARDUINO_AVR_ATTINYX61) || defined(ARDUINO_AVR_ATTINY43) || defined(ARDUINO_AVR_ATTINY828) || defined(ARDUINO_AVR_ATTINY1634) || defined(ARDUINO_AVR_ATTINYX313)
			#include <TinyWireM.h>                  // I2C Master lib for ATTinys which use USI
			#define URTCLIB_WIRE TinyWireM
		#else
			#include <Wire.h>
			#define URTCLIB_WIRE Wire
		#endif
	#endif

	/**
	 * \brief Default RTC I2C address
	 *
	 * Usual address is 0x68
	 */
	#define URTCLIB_ADDRESS 0x68

	/************	MODELS ***********/
	/**
	 * \brief Model definition, DS1307
	 */
	#define URTCLIB_MODEL_DS1307 1
	/**
	 * \brief Model definition, DS3231
	 */
	#define URTCLIB_MODEL_DS3231 2
	/**
	 * \brief Model definition, DS3232
	 */
	#define URTCLIB_MODEL_DS3232 3


	/************	WEEK DAYS ***********/
	/**
	 * \brief Week day definition, [Mon..Sun] as [1..7]. Sunday
	 */
	#define URTCLIB_WEEKDAY_SUNDAY 7
	/**
	 * \brief Week day definition, [Mon..Sun] as [1..7]. Monday
	 */
	#define URTCLIB_WEEKDAY_MONDAY 1
	/**
	 * \brief Week day definition, [Mon..Sun] as [1..7]. Tuesday
	 */
	#define URTCLIB_WEEKDAY_TUESDAY 2
	/**
	 * \brief Week day definition, [Mon..Sun] as [1..7]. Wednesday
	 */
	#define URTCLIB_WEEKDAY_WEDNESDAY 3
	/**
	 * \brief Week day definition, [Mon..Sun] as [1..7]. Thursday
	 */
	#define URTCLIB_WEEKDAY_THURSDAY 4
	/**
	 * \brief Week day definition, [Mon..Sun] as [1..7]. Friday
	 */
	#define URTCLIB_WEEKDAY_FRIDAY 5
	/**
	 * \brief Week day definition, [Mon..Sun] as [1..7]. Saturday
	 */
	#define URTCLIB_WEEKDAY_SATURDAY 6



	/************	ALARM SELECTION: ***********/
	//Note: Not valid for DS1307!

	/**
	 * \brief Alarm 1 - Disabled
	 *
	 * Alarm1 structure:
	 *
	 * bit 0 - A1M1
	 * bit 1 - A1M2
	 * bit 2 - A1M3
	 * bit 3 - A1M4
	 * bit 4 - A1 DT/DY
	 * bit 5 - A1 Enabled
	 * bit 6 - Unused, always 0
	 * bit 7 - Always 0
	 */
	#define URTCLIB_ALARM_TYPE_1_NONE 0b00000000

	/**
	 * \brief Alarm 1 - Trigger every second
	 */
	#define URTCLIB_ALARM_TYPE_1_ALL_S 0b00101111

	/**
	 * \brief Alarm 1 - Trigger every minute at a fixed second
	 */
	#define URTCLIB_ALARM_TYPE_1_FIXED_S 0b00101110

	/**
	 * \brief Alarm 1 - Trigger every hour at a fixed minute and second
	 */
	#define URTCLIB_ALARM_TYPE_1_FIXED_MS 0b00101100

	/**
	 * \brief Alarm 1 - Trigger every day at a fixed hour, minute and second
	 */
	#define URTCLIB_ALARM_TYPE_1_FIXED_HMS 0b00101000

	/**
	 * \brief Alarm 1 - Trigger every month at a fixed day, hour, minute and second
	 */
	#define URTCLIB_ALARM_TYPE_1_FIXED_DHMS 0b00100000

	/**
	 * \brief Alarm 1 - Trigger every week at a fixed day-of-week, hour, minute and second
	 */
	#define URTCLIB_ALARM_TYPE_1_FIXED_DOWHMS 0b00110000

	/**
	 * \brief Alarm 2 - Disabled
	 *
	 * Alarm1 structure:
	 *
	 * bit 0 - A2M1 - Unused, always 0
	 * bit 1 - A2M2
	 * bit 2 - A2M3
	 * bit 3 - A2M4
	 * bit 4 - A2 DT/DY
	 * bit 5 - A2 Enabled
	 * bit 6 - Unused, always 0
	 * bit 7 - Always 1
	 */
	#define URTCLIB_ALARM_TYPE_2_NONE 0b10000000

	/**
	 * \brief Alarm 2 - Trigger every minute at 00 seconds
	 */
	#define URTCLIB_ALARM_TYPE_2_ALL_M 0b10101110

	/**
	 * \brief Alarm 2 - Trigger every hour at minute and 00 seconds
	 */
	#define URTCLIB_ALARM_TYPE_2_FIXED_M 0b10101100

	/**
	 * \brief Alarm 2 - Trigger every day at hour, minute and 00 seconds
	 */
	#define URTCLIB_ALARM_TYPE_2_FIXED_HM 0b10101000

	/**
	 * \brief Alarm 2 - Trigger every month at day, hour, minute and 00 seconds
	 */
	#define URTCLIB_ALARM_TYPE_2_FIXED_DHM 0b10100000

	/**
	 * \brief Alarm 2 - Trigger every week at day-of-week, hour, minute and 00 seconds
	 */
	#define URTCLIB_ALARM_TYPE_2_FIXED_DOWHM 0b10110000


	/**
	 * \brief When requesting for Alarm 1
	 */
	#define URTCLIB_ALARM_1 URTCLIB_ALARM_TYPE_1_NONE

	/**
	 * \brief When requesting for Alarm 2
	 */
	#define URTCLIB_ALARM_2 URTCLIB_ALARM_TYPE_2_NONE

	/**
	 * \brief When requesting for any of both alarms, for triggered alarms
	 */
	#define URTCLIB_ALARM_ANY 0b01010101


	/************	SQWG SELECTION: ***********/

	/**
	 * \brief SQWG OFF, keeps output low
	 *
	 * Only valid for DS1307
	 */
	#define URTCLIB_SQWG_OFF_0 0b11111111

	/**
	 * \brief SQWG OFF, keeps output hight
	 */
	#define URTCLIB_SQWG_OFF_1 0b11111110

	/**
	 * \brief SQWG running at 1Hz
	 */
	#define URTCLIB_SQWG_1H 0b00000000

	/**
	 * \brief SQWG running at 1024Hz
	 *
	 * Not  valid for DS1307
	 */
	#define URTCLIB_SQWG_1024H 0b00001000

	/**
	 * \brief SQWG running at 4096Hz
	 */
	#define URTCLIB_SQWG_4096H 0b00010000

	/**
	 * \brief SQWG running at 8192Hz
	 */
	#define URTCLIB_SQWG_8192H 0b00011000

	/**
	 * \brief SQWG running at 32768Hz
	 *
	 * Only valid for DS1307
	 */
	#define URTCLIB_SQWG_32768H 0b00000011


	/************	TEMPERATURE ***********/
	/**
	 * \brief Temperarure read error indicator return value
	 *
	 * 327.67º, obviously erroneous
	 */
	#define URTCLIB_TEMP_ERROR 32767


	/************	MISC  ***********/


	/**
	 * \brief Convert normal decimal numbers to binary coded decimal
	 */
	#define uRTCLIB_decToBcd(val) ((uint8_t) ((val / 10 * 16) + (val % 10)))

	/**
	 * \brief Convert binary coded decimal to normal decimal numbers
	 */
	#define uRTCLIB_bcdToDec(val) ((uint8_t) ((val / 16 * 10) + (val % 16)))

	// ESP yield function (ESP32 has no need for that on dual core, but it has on single core version)
	#if ARDUINO_ARCH_ESP8266
		/**
		 * \brief ESP8266, yield to don't block ESP functionality.
		 *
		 * When this library is used in other MCUs this is simply removed by the preprocessor
		 */
		#define uRTCLIB_YIELD yield();
	#else
		#if ARDUINO_ARCH_ESP32
		/**
		 * \brief ESP32, yield to don't block ESP functionality.
		 *
		 * When this library is used in other MCUs this is simply removed by the preprocessor
		 */
			#define uRTCLIB_YIELD yield();
		#else
			#define uRTCLIB_YIELD
		#endif
	#endif

	class uRTCLib {
		public:
			/******* Constructors *******/
			uRTCLib();
			uRTCLib(const int);
			uRTCLib(const int, const uint8_t);

			/******* RTC functions ********/
			void refresh();
			uint8_t second();
			uint8_t minute();
			uint8_t hour();
			uint8_t day();
			uint8_t month();
			uint8_t year();
			uint8_t dayOfWeek();
			int16_t temp();
			void set(const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t);
			void set_rtc_address(const int);
			void set_model(const uint8_t);
			uint8_t model();

			/******* Power ********/
			bool lostPower();
			void lostPowerClear();
			bool enableBattery(); // Only DS3231 and DS3232.
			bool disableBattery(); // Only DS3231 and DS3232.


			/******** Alarms ************/
			bool alarmSet(const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t); // Seconds will be ignored on Alarm 2
			bool alarmDisable(const uint8_t);
			bool alarmClearFlag(const uint8_t);
			uint8_t alarmMode(const uint8_t);
			uint8_t alarmSecond(const uint8_t);
			uint8_t alarmMinute(const uint8_t);
			uint8_t alarmHour(const uint8_t);
			uint8_t alarmDayDow(const uint8_t);
			bool alarmTriggered(const uint8_t);

			/*********** SQWG ************/
			uint8_t sqwgMode();
			bool sqwgSetMode(const uint8_t);


			/************ RAM *************/
			// Only DS1307 and DS3232.
			// DS1307: Addresses 08h to 3Fh so we offset 08h positions and limit to 38h as maximum address
			// DS3232: Addresses 14h to FFh so we offset 14h positions and limit to EBh as maximum address
			byte ramRead(const uint8_t);
			bool ramWrite(const uint8_t, byte);

			/************ Aging *************/
			// Only DS3231 and DS3232. Address 0x10h
			int8_t agingGet();
			bool agingSet(int8_t);

			/************ 32K Generator pin *************/
			// Only DS3231 and DS3232. On DS1307 we map it to SqWG
			bool enable32KOut();
			bool disable32KOut();
			bool status32KOut();


		private:
			// Address
			int _rtc_address = URTCLIB_ADDRESS;

			// RTC read data
			uint8_t _second = 0;
			uint8_t _minute = 0;
			uint8_t _hour = 0;
			uint8_t _day = 0;
			uint8_t _month = 0;
			uint8_t _year = 0;
			uint8_t _dayOfWeek = 0;
			int16_t _temp = 9999;

			// Model, for alarms and RAM
			uint8_t _model = URTCLIB_MODEL_DS3232;

			// Alarms:
			uint8_t _a1_mode = URTCLIB_ALARM_TYPE_1_NONE;
			uint8_t _a1_second = 0;
			uint8_t _a1_minute = 0;
			uint8_t _a1_hour = 0;
			uint8_t _a1_day_dow = 0;
			bool _a1_triggered_flag = false;

			uint8_t _a2_mode = URTCLIB_ALARM_TYPE_2_NONE;
			uint8_t _a2_minute = 0;
			uint8_t _a2_hour = 0;
			uint8_t _a2_day_dow = 0;
			bool _a2_triggered_flag = false;

			// Aging
			int8_t _aging = 0;

			// SQWG
			uint8_t _sqwg_mode = URTCLIB_SQWG_OFF_1;

			// OSC failed Flag
			bool _lost_power = false;

			// 32K output Flag
			bool _32k = false;

	};

#endif


