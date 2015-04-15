# RTCLib
RTCLib

## What is this repository for? ##

Really tiny library to basic RTC and EEPROM (incorporated) functionality on Arduino.

DS1307 and DS3231 RTCs are supported
AT24C32 EEPROM supported

(and compatibles)


## How do I get set up? ##

 * Get the ZIP.
 * Rename to RTCLib.zip
 * Install library on Arduino

 * Optionally you can edit RTCLib.h to change I2C addresses or disable RTC setting functionality or EEPROM functionality,


## Example ##



```
#!Arduino


#include <Wire.h>
#include <RTCLib.h>


RTCLib rtc;
unsigned int pos;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial OK");
//  Max position: 32767
  for(pos = 0; pos < 1000; pos++) {
    rtc.eeprom_write(pos, (unsigned char) pos % 256);
  }
// Only used once, then disabled
//  rtc.set(0, 8, 17, 1, 5, 4, 15);
  // RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

  pos = 0;
}

void loop() {
  rtc.refresh();
  Serial.println();
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
  delay(99);
}
```



## Who do I talk to? ##

 * @Naguissa
 * http://www.naguissa.com
