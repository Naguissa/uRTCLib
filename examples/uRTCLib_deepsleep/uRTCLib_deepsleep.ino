/**
 * DS1307, DS3231 and DS3232 RTCs basic library
 *
 * Really tiny library to basic RTC functionality on Arduino.
 *
 * Supported features:
 *     * SQuare Wave Generator
 *     * Fixed output pin for DS1307
 *     * RAM for DS1307 and DS3232
 *     * temperature sensor for DS3231 and DS3232
 *     * Alarms (1 and 2) for DS3231 and DS3232
 *     * Power failure check for DS3231 and DS3232
 *
 * See uEEPROMLib for EEPROM support.
 *
 *
 *  INPORTANT: Place a 10uF capacitor between RTC's SQW pin and microcontroller input pin. If it's electrolytic, place - on microcontroller pin.
 *
 *
 * THIS SKETCH COMPATIBILITY LIST:
 *
 *  * AVR (tested) and 32U4 - SQW ----> RST
 *  * ESP8266 - SQW ---> RST
 *  * ESP32 - SQW ---> GPIO_33 (D33), RST can also be used, but you loose RTC SRAM functionality
 *
 *
 * See here how to connect everything: https://raw.githubusercontent.com/Naguissa/uRTCLib_doc_and_extras/master/imgs/deepsleep_connection.png
 *
 *
 * Before reporting any problem uncomment rtc.set line and make sure to test it using proper resistors on I2C:
 *
 * For 5V bus
 *  * Connect a 4.7k resistor between SDA and Vcc
 *  * Connect a 4.7k resistor between SCL and Vcc
 * For 3.3V bus
 *  * Connect a 2.4k resistor between SDA and Vcc
 *  * Connect a 2.4k resistor between SCL and Vcc
 *
 *
 * @copyright Naguissa
 * @author Naguissa
 * @url https://github.com/Naguissa/uRTCLib
 * @url https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html
 * @email naguissa@foroelectro.net
 */
#include "Arduino.h"
#include "uRTCLib.h"

#if defined(__AVR_ATmega32U4__) || defined(ARDUINO_ARCH_AVR)
	#include <avr/sleep.h>
	#include <avr/power.h>
#endif

uRTCLib rtc;


#ifdef ARDUINO_ARCH_ESP32
	RTC_DATA_ATTR int bootCount = 0;
#endif



void setup() {
delay (2000);
	Serial.begin(9600);
	Serial.println("Serial OK");

	#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32) || defined(__AVR_ATmega32U4__) || defined(ARDUINO_ARCH_AVR)
		#ifdef ARDUINO_ARCH_ESP8266
			URTCLIB_WIRE.begin(0, 2); // D3 and D4 on ESP8266
		#else
			URTCLIB_WIRE.begin();
		#endif

		#ifdef ARDUINO_ARCH_ESP32
			++bootCount;
			Serial.print("Boot counter: ");
			Serial.println(bootCount);
		#endif




		rtc.set_rtc_address(0x68);
		rtc.set_model(URTCLIB_MODEL_DS3232);

		// Clear alarm to reuse it later
		rtc.alarmClearFlag(URTCLIB_ALARM_1);


		// Uncomment this if RTC is not set:
		// rtc.set(0, 0, 0, 1, 1, 6, 20);  // RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)


		// Alarm pin will get low at any :10 seconds
		rtc.alarmSet(URTCLIB_ALARM_TYPE_1_FIXED_S, 20, 0, 0, 1); // Each minute, at just :20 seconds
			// RTCLib::alarmSet(uint8_t type, uint8_t second, uint8_t minute, uint8_t hour, uint8_t day_dow);



		// Go deep sleep:

		delay(1000);

		#ifdef ARDUINO_ARCH_ESP8266
			ESP.deepSleep(0); // 0 = sleep forever
		#endif

		#ifdef ARDUINO_ARCH_ESP32
			esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); //1 = High, 0 = Low
			esp_deep_sleep_start();
		#endif

		#if defined(__AVR_ATmega32U4__) || defined(ARDUINO_ARCH_AVR)
			set_sleep_mode (SLEEP_MODE_PWR_DOWN);
			sleep_enable();
			sleep_cpu ();
		#endif


		// END

		delay(1000); // Should never be executed
	#else
		Serial.println("This example does not work with current microcontroller, see compatibility list on file header.");
	#endif




}

void loop() {
}
