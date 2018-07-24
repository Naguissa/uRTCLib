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

	delay(2500);
	Serial.println("Delay OK");

	#ifdef ARDUINO_ARCH_ESP8266
		Wire.begin(0, 2); // D3 and D4 on ESP8266
	#else
		Wire.begin();
	#endif


#ifdef ARDUINO_ARCH_AVR
	int inttmp = 32123;
#else
	// too logng for AVR 16 bits!
	int inttmp = 24543557;
#endif
	float floattmp = 3.1416;
	char chartmp = 'A';

    char string[17] = "ForoElectro.Net\0";


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

	if (!rtc.eeprom_write(9, (byte *) &string[0], 16)) {
		Serial.println("Failed to store STRING");
	} else {
		Serial.println("STRING correctly stored");
	}

	inttmp = 0;
	floattmp = 0;
	chartmp = 0;
    string[0] = string[1] = string[2] = string[3] = string[4] = 0;



	Serial.print("int: ");
	rtc.eeprom_read(0, &inttmp);
	Serial.println(inttmp);

	Serial.print("float: ");
	rtc.eeprom_read(4, &floattmp);
	Serial.println((float) floattmp);

	Serial.print("char: ");
	rtc.eeprom_read(8, &chartmp);
	Serial.println(chartmp);

	Serial.print("chararray: ");
	rtc.eeprom_read(9, (byte *) &string[0], 16);
	Serial.println(string);

	Serial.println();


	for(pos = 26; pos < 1000; pos++) {
		rtc.eeprom_write(pos, (unsigned char) (pos % 256));
	}

	pos = 0;
}

void loop() {
	Serial.print(pos);
	Serial.print(": ");
	Serial.print(rtc.eeprom_read(pos));

	Serial.println();

	pos++;
	pos %= 1000;
	delay(1000);
}
