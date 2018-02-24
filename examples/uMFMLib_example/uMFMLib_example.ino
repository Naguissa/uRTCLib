#include "Arduino.h"
#include "uMFMLib.h"


// uRTCLib rtc;
uMFMLib lib;


bool toEncode[8] = {0, 0, 0, 1, 1, 0, 1, 1};


void encodeLoop() {
	unsigned char encodedClock, encodedData;
	unsigned char idx;
	unsigned char ret;

	for (idx = 0; idx < 8; idx++) {
		Serial.print(' ');
		ret = lib.encode(toEncode[idx]);
		encodedClock = (bool) ((ret & B00000010) >> 1);
		encodedData = (bool) (ret & B00000001);

		Serial.print((unsigned int) encodedClock);
		Serial.print((unsigned int) encodedData);
	}
	Serial.println();
}

void decodeFM() {
	bool toDecode[16] = {1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1};
	unsigned char decoded;
	unsigned char idx;
	unsigned char ret;

	for (idx = 0; idx < 16; idx++) {
		Serial.print(' ');

		// Data
		decoded = lib.decode(toDecode[idx]);
		decoded &= B00000001;

		if (idx % 2 == 0) { // Clock
			continue;
		}

		Serial.print((unsigned int) decoded);
	}
	Serial.println();
}

void decodeMFM() {
	bool toDecode[16] = {0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1};
	unsigned char decoded;
	unsigned char idx;
	unsigned char ret;

	for (idx = 0; idx < 16; idx++) {
		Serial.print(' ');

		if (idx % 2 == 0) { // Clock
			continue;
		}

		// Data
		decoded = lib.decode(toDecode[idx]);
		decoded &= B00000001;

		Serial.print((unsigned int) decoded);
	}
	Serial.println();
}



void setup() {
	delay (2000);
	Serial.begin(57600);
	Serial.println("Serial OK");

	lib.setType(UMFMFLIB_TYPE_FM);
	lib.reset();
	Serial.print("FM:");
	encodeLoop();

	lib.setType(UMFMFLIB_TYPE_MFM);
	lib.reset();
	Serial.print("MFM:");
	encodeLoop();

	lib.setType(UMFMFLIB_TYPE_FM);
	lib.reset();
	Serial.print("FM:");
	decodeFM();

	lib.setType(UMFMFLIB_TYPE_MFM);
	lib.reset();
	Serial.print("MFM:");
	decodeMFM();


	// Wait for sync:
	lib.reset();
	bool checkSync[20] = {1,1,1,0,0,1,0,0,0,1,0,1,1,0,0,0,1,0,0,1};
	unsigned char idx;
	Serial.print("No SYNC:");
	for (idx = 0; idx < 20; idx++) {
		Serial.print(' ');
		Serial.print((unsigned int) lib.waitForSync(checkSync[idx]));
	}
	Serial.println();

	lib.reset();
	Serial.print("SYNC no reset:");
	for (idx = 0; idx < 15; idx++) {
		Serial.print(' ');
		Serial.print((unsigned int) lib.waitForSync(lib.syncMark[idx]));
	}
	Serial.println();

	lib.reset();
	Serial.print("reset + SYNC:");
	for (idx = 0; idx < 15; idx++) {
		Serial.print(' ');
		Serial.print((unsigned int) lib.waitForSync(lib.syncMark[idx]));
	}
	Serial.println();


	Serial.print("+ values after sync (I):");
	Serial.print(' ');
	Serial.print((unsigned int) lib.waitForSync(1));
	Serial.print(' ');
	Serial.print((unsigned int) lib.waitForSync(0));
	Serial.print(' ');
	Serial.print((unsigned int) lib.waitForSync(0));
	Serial.print(' ');
	Serial.print((unsigned int) lib.waitForSync(1));
	Serial.print(' ');
	Serial.print((unsigned int) lib.waitForSync(1));
	Serial.print(' ');
	Serial.print((unsigned int) lib.waitForSync(0));
	Serial.println();

	Serial.print("+ values after sync (II):");
	for (idx = 0; idx < 15; idx++) {
		Serial.print(' ');
		Serial.print((unsigned int) lib.waitForSync(lib.syncMark[idx]));
	}
	Serial.println();

}


void loop() {
}
