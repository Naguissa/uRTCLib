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
 * @copyright Naguissa
 * @author Naguissa / ArminJo
 * @url https://github.com/Naguissa/uRTCLib
 * @url https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html
 * @email naguissa@foroelectro.net
 */
#include "Arduino.h"
#include "uRTCLib.h"

uRTCLib rtc;

#define VERSION_EXAMPLE "1.0"

#define DATE_FORMAT_LONG_MASK 0x01
#define DATE_FORMAT_AMERICAN 0x02
#define DATE_FORMAT_AMERICAN_LONG 0x03
#define DATE_FORMAT_EUROPEAN 0x04
#define DATE_FORMAT_EUROPEAN_LONG 0x05

void printRTC(uint8_t aDateFormatSpecifier = 0);
bool printRTCEveryPeriod(uint16_t aPeriodSeconds, uint8_t aDateFormatSpecifier = 0);

void printRTCDate(uint8_t aDateFormatSpecifier = 0);
void printRTCDateAmericanFormat(bool aPrintLongFormat);
void printRTCDateEuropeanFormat(bool aPrintLongFormat);
void printRTCDateISOFormat();

void printRTCTime(bool aPrintLongFormat = true, bool aDoRefresh = false);

void printRTCTemperature();
bool requestDateBySerialAndSet(void);

#if !defined(__AVR__) && ! defined(PROGMEM)
#define PROGMEM
#endif

//#define GERMAN_NAMES_FOR_DATE

#if defined(GERMAN_NAMES_FOR_DATE)
const char Sunday[] PROGMEM= "Sonntag";
const char Monday[] PROGMEM= "Montag";
const char Tuesday[] PROGMEM= "Dienstag";
const char Wednesday[] PROGMEM= "Mittwoch";
const char Thursday[] PROGMEM= "Donnerstag";
const char Friday[] PROGMEM= "Freitag";
const char Saturday[] PROGMEM= "Samstag";

const char January[] PROGMEM= "Januar";
const char February[] PROGMEM= "Februar";
const char March[] PROGMEM= "März";
const char April[] PROGMEM= "April";
const char May[] PROGMEM= "Mai";
const char June[] PROGMEM= "Juni";
const char July[] PROGMEM= "July";
const char August[] PROGMEM= "August";
const char September[] PROGMEM= "September";
const char October[] PROGMEM= "Oktober";
const char November[] PROGMEM= "November";
const char December[] PROGMEM= "Dezember";
#else
const char Sunday[] PROGMEM ="Sunday";
const char Monday[] PROGMEM ="Monday";
const char Tuesday[] PROGMEM ="Tuesday";
const char Wednesday[] PROGMEM ="Wednesday";
const char Thursday[] PROGMEM ="Thursday";
const char Friday[] PROGMEM ="Friday";
const char Saturday[] PROGMEM ="Saturday";

const char January[] PROGMEM ="January";
const char February[] PROGMEM = "February";
const char March[] PROGMEM = "March";
const char April[] PROGMEM = "April";
const char May[] PROGMEM = "May";
const char June[] PROGMEM = "June";
const char July[] PROGMEM = "July";
const char August[] PROGMEM = "August";
const char September[] PROGMEM = "September";
const char October[] PROGMEM = "October";
const char November[] PROGMEM ="November";
const char December[] PROGMEM = "December";
#endif

const char* const sDayStrings[] PROGMEM = { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };
#define DOW_MAX_STRING_SIZE 11 // excluding trailing NUL (Donnerstag)

const char* const sMonthStrings[] PROGMEM = { January, February, March, April, May, June, July, August, September, October,
		November, December };
#define MONTH_MAX_STRING_SIZE 10 // excluding trailing NUL (September)

void setup() {
	Serial.begin(115200);
	while (!Serial)
		; //delay for Leonardo
		  // Just to know which program is running on my Arduino
	Serial.println(F("START " __FILE__ "\r\nVersion " VERSION_EXAMPLE " from " __DATE__));

	#ifdef ARDUINO_ARCH_ESP8266
		URTCLIB_WIRE.begin(0, 2); // D3 and D4 on ESP8266
	#else
		URTCLIB_WIRE.begin();
	#endif

	rtc.set_rtc_address(0x68);
	rtc.set_model(URTCLIB_MODEL_DS3232);

	// Only used once, then disabled
	rtc.set(0, 42, 16, 6, 2, 5, 15);
	//  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

	if (rtc.lostPower() || rtc.year() < 19) {
		if (requestDateBySerialAndSet()) {
			rtc.lostPowerClear();
		}
	}
}

void loop() {

	/*
	 * Print date and time each second in 5 different formats
	 */
	if (printRTCEveryPeriod(1)) {
		printRTC(DATE_FORMAT_EUROPEAN);
		printRTC(DATE_FORMAT_EUROPEAN_LONG);
		printRTC(DATE_FORMAT_AMERICAN);
		printRTC(DATE_FORMAT_AMERICAN_LONG);
		Serial.println();
	}
	delay(1000);
}

bool requestDateBySerialAndSet(void) {

	uint8_t tSecond, tMinute, tHour, tDayOfMonth, tMonth, tYear;
	char tInputBuffer[17];
	Serial.println();
	Serial.println(F("Enter the time in format: \"HH MM SS DD MM YY\" - Timeout is 30 seconds"));
	Serial.setTimeout(30000);
	// read exactly 17 characters
	size_t tReadLength = Serial.readBytes(tInputBuffer, 17);
	if (tReadLength == 17 && tInputBuffer[14] == ' ') {
		#ifdef AVR
			sscanf_P(tInputBuffer, PSTR("%2hhu %2hhu %2hhu %2hhu %2hhu %2hhu"), &tHour, &tMinute, &tSecond, &tDayOfMonth, &tMonth, &tYear);
		#else
			sscanf(tInputBuffer, "%2hhu %2hhu %2hhu %2hhu %2hhu %2hhu", &tHour, &tMinute, &tSecond, &tDayOfMonth, &tMonth, &tYear);
		#endif
		// read newline etc.
		while (Serial.available()) {
			Serial.read();
		}
		Serial.setTimeout(10000);
		Serial.println();
		Serial.println(F("Enter the day of week as number between 1 and 7 (Sunday-Saturday) - Timeout is 10 seconds"));
		Serial.readBytes(tInputBuffer, 1);
		if (tInputBuffer[0] > '0' && tInputBuffer[0] < '8') {
			rtc.set(tSecond, tMinute, tHour, tInputBuffer[0] - '0', tDayOfMonth, tMonth, tYear);
			Serial.print(F("Time set to: "));
			printRTC();
			return true;
		}
	}
	Serial.println(F("Clock has not been changed, press reset for next try."));
	return false;
}

/*
 * @return true if update/refresh happened
 */
bool printRTCEveryPeriod(uint16_t aPeriodSeconds, uint8_t aDateFormatSpecifier) {
	static long sLastMillisOfRTCRead;
	if (millis() - sLastMillisOfRTCRead >= (1000 * aPeriodSeconds)) {
		sLastMillisOfRTCRead = millis();
		rtc.refresh();
		printRTC(aDateFormatSpecifier);
		return true;
	}
	return false;
}

void printRTC(uint8_t aDateFormatSpecifier) {
	rtc.refresh();
	printRTCDate(aDateFormatSpecifier);
	Serial.print(' ');
	printRTCTime((aDateFormatSpecifier & DATE_FORMAT_LONG_MASK), false);
	Serial.println();
}

void printRTCDateAmericanFormat(bool aPrintLongFormat) {
#if defined(__AVR__)
	char tDateString[(2 * DOW_MAX_STRING_SIZE) + 1 + (2 * MONTH_MAX_STRING_SIZE) + 1 + 12];
#else
	char tDateString[DOW_MAX_STRING_SIZE + MONTH_MAX_STRING_SIZE + 12];
#endif

	if (aPrintLongFormat) {
#if defined(__AVR__)
		// fist copy day of week
		strcpy_P(&tDateString[sizeof(tDateString) - (DOW_MAX_STRING_SIZE + MONTH_MAX_STRING_SIZE) - 3],
				(PGM_P) pgm_read_word(&sDayStrings[rtc.dayOfWeek() - 1]));
		strcpy_P(&tDateString[sizeof(tDateString) - (MONTH_MAX_STRING_SIZE) - 2],
				(PGM_P) pgm_read_word(&sMonthStrings[rtc.month() - 1]));
		sprintf_P(tDateString, PSTR("%s, %s %2hhu, 20%2hhu"),
				&tDateString[sizeof(tDateString) - (DOW_MAX_STRING_SIZE + MONTH_MAX_STRING_SIZE) - 3],
				&tDateString[sizeof(tDateString) - (MONTH_MAX_STRING_SIZE) - 2], rtc.day(), rtc.year());
#else
		sprintf(tDateString, "%s, %s %2u, 20%2u", sDayStrings[rtc.dayOfWeek() - 1],
				sMonthStrings[rtc.month() - 1], rtc.day(), rtc.year());
#endif
	} else {
		sprintf_P(tDateString, PSTR("%02u/%02u/20%2u"), rtc.month(), rtc.day(), rtc.year());
	}
	Serial.print(tDateString);
}

void printRTCDateEuropeanFormat(bool aPrintLongFormat) {
#if defined(__AVR__)
	char tDateString[(2 * DOW_MAX_STRING_SIZE) + 1 + (2 * MONTH_MAX_STRING_SIZE) + 1 + 12];
#else
	char tDateString[DOW_MAX_STRING_SIZE + MONTH_MAX_STRING_SIZE + 12];
#endif

	if (aPrintLongFormat) {
#if defined(__AVR__)
		// fist copy day of week
		strcpy_P(&tDateString[sizeof(tDateString) - (DOW_MAX_STRING_SIZE + MONTH_MAX_STRING_SIZE) - 3],
				(PGM_P) pgm_read_word(&sDayStrings[rtc.dayOfWeek() - 1]));
		strcpy_P(&tDateString[sizeof(tDateString) - (MONTH_MAX_STRING_SIZE) - 2],
				(PGM_P) pgm_read_word(&sMonthStrings[rtc.month() - 1]));
		sprintf_P(tDateString, PSTR("%s, %2hhu. %s 20%2hhu"),
				&tDateString[sizeof(tDateString) - (DOW_MAX_STRING_SIZE + MONTH_MAX_STRING_SIZE) - 3], rtc.day(),
				&tDateString[sizeof(tDateString) - (MONTH_MAX_STRING_SIZE) - 2], rtc.year());
#else
		sprintf(tDateString, "%s, %2u. %s 20%2u", sDayStrings[rtc.dayOfWeek() - 1], rtc.day(),
				sMonthStrings[rtc.month() - 1], rtc.year());
#endif
	} else {
		sprintf_P(tDateString, PSTR("%02u.%02u.20%2u"), rtc.day(), rtc.month(), rtc.year());
	}
	Serial.print(tDateString);
}

void printRTCDateISOFormat() {
	char tDateString[11];
#if defined(__AVR__)
	sprintf_P(tDateString, PSTR("20%2hhu-%02hhu-%02hhu"), rtc.year(), rtc.month(), rtc.day());
#else
	sprintf(tDateString, "20%2u-%02u-%02u", rtc.year(), rtc.month(), rtc.day());
#endif
	Serial.print(tDateString);
}

void printRTCDate(uint8_t aDateFormatSpecifier) {

	if (aDateFormatSpecifier & DATE_FORMAT_AMERICAN) {
		printRTCDateAmericanFormat((aDateFormatSpecifier & DATE_FORMAT_LONG_MASK));
	} else if (aDateFormatSpecifier & DATE_FORMAT_EUROPEAN) {
		printRTCDateEuropeanFormat((aDateFormatSpecifier & DATE_FORMAT_LONG_MASK));
	} else {
// ISO Format
		printRTCDateISOFormat();
	}
}

void printRTCTemperature() {
	Serial.print(rtc.temp() / 100);
	Serial.print('.');
	Serial.print(rtc.temp() - ((rtc.temp() / 100) * 100));
}

void printRTCTime(bool aPrintLongFormat, bool aDoRefresh) {
	if (aDoRefresh) {
		rtc.refresh();
	}
	char tTimeString[9]; // 8 + trailing NUL character
#if defined(__AVR__)
	if (aPrintLongFormat) {
		sprintf_P(tTimeString, PSTR("%02hhu:%02hhu:%02hhu"), rtc.hour(), rtc.minute(), rtc.second());
	} else {
		sprintf_P(tTimeString, PSTR("%02u:%02u"), rtc.hour(), rtc.minute());
	}
#else
	if (aPrintLongFormat) {
		sprintf(tTimeString, "%02u:%02u:%02u", rtc.hour(), rtc.minute(), rtc.second());
	} else {
		sprintf(tTimeString, "%02u:%02u", rtc.hour(), rtc.minute());
	}
#endif
	Serial.print(tTimeString);
}
