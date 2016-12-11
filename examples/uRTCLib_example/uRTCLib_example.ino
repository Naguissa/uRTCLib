#include "Arduino.h"
#include "Wire.h"
#include "RTCLib.h"


RTCLib rtc;


unsigned int pos;

void setup() {
delay (2000);
  Serial.begin(9600);
  Serial.println("Serial OK");
//  Max position: 32767

  for(pos = 0; pos < 1000; pos++) {
    rtc.eeprom_write(pos, (unsigned char) pos % 256);
  }

// Only used once, then disabled
//  rtc.set(0, 42, 16, 6, 2, 5, 15);
//  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

  pos = 0;
  
  #ifdef _VARIANT_ARDUINO_STM32_
  Serial.println("Board: Maple");
  #else
  Serial.println("Board: Other");
  #endif
  
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
