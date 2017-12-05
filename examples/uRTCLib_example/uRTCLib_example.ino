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

	// Testing template
	rtc.eeprom_write(0, (int) 32101);
	rtc.eeprom_write(2, 3.14159);
	rtc.eeprom_write(6, 'A');

	for(pos = 7; pos < 1000; pos++) {
		rtc.eeprom_write(pos, (unsigned char) pos % 256);
	}

	// Only used once, then disabled
	//  rtc.set(0, 42, 16, 6, 2, 5, 15);
	//  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

	pos = 7;

	#ifdef _VARIANT_ARDUINO_STM32_
		Serial.println("Board: STM32");
	#else
		Serial.println("Board: Other");
	#endif
 
	Serial.print("int: ");
	Serial.print((int) rtc.eeprom_read(0));
	Serial.print("; float: ");
	Serial.print((float) rtc.eeprom_read(2));
	Serial.print("; char: ");
	Serial.print((char) rtc.eeprom_read(6));
	Serial.println();
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
	Serial.print(" ---- ");
	Serial.print(pos);
	Serial.print(": ");
	Serial.print(rtc.eeprom_read(pos));

	Serial.println();
	pos++;
	pos %= 1000;
	delay(1000);
}
