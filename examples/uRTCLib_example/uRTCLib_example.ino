#include "Arduino.h"
#include "Wire.h"
#include "uRTCLib.h"


// uRTCLib rtc;
uRTCLib rtc(0x68, 0x57);



unsigned int pos;

void setup() {
delay (2000);
	Serial.begin(9600);
	Serial.println("Serial OK");
	//  Max position: 32767

	#ifdef ARDUINO_ARCH_ESP8266
		Wire.begin(0, 2); // D3 and D4 on ESP8266
	#else
		Wire.begin();
	#endif


	#ifdef _VARIANT_ARDUINO_STM32_
		Serial.println("Board: STM32");
	#else
		Serial.println("Board: Other");
	#endif
#ifdef ARDUINO_ARCH_AVR
	int inttmp = 32123;
#else
	// too logng for AVR 16 bits!
	int inttmp = 24543557;
#endif
	float floattmp = 3.1416;
	char chartmp = 'A';

	// Testing template
	if (!rtc.eeprom_write(0, inttmp)) {
		Serial.println("Failed to store INT");
	} else {
		Serial.println("INT correctly stored");
	}
	if (!rtc.eeprom_write(4, floattmp)) {
		Serial.println("Failed to store FLOAT");
	} else {
		Serial.println("FLOAT correctly stored");
	}
	if (!rtc.eeprom_write(8, chartmp)) {
		Serial.println("Failed to store CHAR");
	} else {
		Serial.println("CHAR correctly stored");
	}

	rtc.set(0, 42, 16, 6, 2, 5, 15);
	//  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)


	inttmp = 0;
	floattmp = 0;
	chartmp = 0;



	Serial.print("int: ");
	rtc.eeprom_read(0, &inttmp);
	Serial.println(inttmp);
	Serial.print("float: ");
	rtc.eeprom_read(4, &floattmp);
	Serial.println((float) floattmp);
	Serial.print("char: ");
	rtc.eeprom_read(8, &chartmp);
	Serial.println(chartmp);
	Serial.println();


	for(pos = 9; pos < 1000; pos++) {
		rtc.eeprom_write(pos, (unsigned char) (pos % 256));
	}

	pos = 0;
}

void loop() {
	rtc.refresh();

	Serial.print("RTC DateTime: ");
	Serial.print(rtc.year());
	Serial.print('/');
	Serial.print(rtc.month());
	Serial.print('/');
	Serial.print(rtc.day());

	Serial.print(' ');

	Serial.print(rtc.hour());
	Serial.print(':');
	Serial.print(rtc.minute());
	Serial.print(':');
	Serial.print(rtc.second());

	Serial.print(" DOW: ");
	Serial.print(rtc.dayOfWeek());

	Serial.print(" - Temp: ");
	Serial.print(rtc.temp());

	Serial.print(" ---- ");
	Serial.print(pos);
	Serial.print(": ");
	Serial.print(rtc.eeprom_read(pos));

	Serial.println();

	pos++;
	pos %= 1000;
	delay(1000);
}
