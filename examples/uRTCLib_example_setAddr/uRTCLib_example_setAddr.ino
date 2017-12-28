#include "Arduino.h"
#include "Wire.h"
#include "uRTCLib.h"


uRTCLib rtc;


unsigned int pos;

void setup() {
delay (2000);
	Serial.begin(9600);
	Serial.println("Serial OK");
	//  Max position: 32767

	// Wire.begin(0, 2); // D3 and D4 on ESP8266
	Wire.begin();

	for(pos = 0; pos < 1000; pos++) {
		rtc.eeprom_write(pos, (unsigned char) pos % 256);
	}

	// Only used once, then disabled
	//  rtc.set(0, 42, 16, 6, 2, 5, 15);
	//  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

	pos = 0;

	#ifdef _VARIANT_ARDUINO_STM32_
	Serial.println("Board: STM32");
	#else
	Serial.println("Board: Other");
	#endif
	rtc.set_rtc_address(0x68);
	rtc.set_ee_address(0x57);
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
