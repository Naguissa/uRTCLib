/**
 * DS1307 and DS3231 RTCs with AT24C32 (and compatible) integrated EEPROM basic library
 *
 * Really tiny library to basic RTC and EEPROM (incorporated) functionality on Arduino.
 *
 * DS1307 and DS3231 RTCs are supported AT24C32 EEPROM supported (and compatibles). Also temperature sensor is supported for DS3231.
 *
 *
 * @copyright Naguissa
 * @author Naguissa
 * @email naguissa.com@gmail.com
 * @version 3.2.0
 * @created 2015-05-07
 */
#include <Arduino.h>
#include "uMFMLib.h"

/**
 * Constructor
 */
uMFMLib::uMFMLib() {
	reset();
}


/**
 * Resets previous value
 */
void uMFMLib::reset() {
	_last = 1;
	_lastType = UMFMFLIB_LASTTYPE_DATA;
	_syncState = 0;
}



/**
 * Changes codification type
 *
 * WARNING: Currently only DS3231 is known to have it at a known address
 *
 * @param unsigned char Desired codification type
 */
void uMFMLib::setType(unsigned char t) {
	_type = t;
	reset();
}

/**
 * Encodes one bit in corresponding two bits
 *
 * @param bool Bit to encode
 * @return unsigned char The two ending bits are the encoded bits
 */
unsigned char uMFMLib::encode(bool i) {
	unsigned char ret;
	if (_type == UMFMFLIB_TYPE_FM) {
		ret = B00000010 | i;
	} else {
		if (i == 1) { // 1 is always 01
			ret = B00000001;
		} else if (_last == 0) {
			ret = B00000010;
		} else {
			ret = B00000000;
		}
	}
	_last = i;
	return ret;
}

/**
 * Decodes one bit
 *
 * You need to feed always two bits in {clock, data} order.
 * If bit corresponds to clock, B00000000 is returned.
 * If bit corresponds to data, crrect data is returned.
 * No ckecks are done, so if it's invalid will return unexpected results.
 * Process is simple: In both encodings data bit correspond to actual data, so simple ignore clock bits.
 *
 * @param bool Bit to encode
 * @return unsigned char The two ending bits are the encoded bits
 */
unsigned char uMFMLib::decode(bool i) {
	unsigned char ret;
	if (_lastType == UMFMFLIB_LASTTYPE_DATA) {
		_lastType = UMFMFLIB_LASTTYPE_CLOCK;
		ret = B00000000;
	} else {
		_lastType = UMFMFLIB_LASTTYPE_DATA;
		ret = B00000010 | i;
	}
	return ret;
}

bool uMFMLib::waitForSync(bool i) {
	bool ret = false;

	// State machine checking coding depending in actual bit + previous ones (these ones are state-dependant, not in memory)
	switch (_syncState) {
		case 15: // ended
			break;

		case 14: // 1
			if (syncMark[_syncState] == i) {
				ret = true;
				_syncState++;
			} else {
				_syncState = 4;
			}
			break;

		case 7: // 1
			if (syncMark[_syncState] == i) {
				_syncState++;
			} else {
				_syncState = 4;
			}
			break;

		case 11: // 1
		case 4: // 1
			if (syncMark[_syncState] == i) {
				_syncState++;
			} else {
				_syncState = 0;
			}
			break;

		case 13: // 0
		case 12: // 0
		case 10: // 0
		case 9: // 0
		case 8: // 0
		case 6: // 0
		case 5: // 0
		case 3: // 0
		case 2: // 0
		case 1: // 0
			if (syncMark[_syncState] == i) {
				_syncState++;
			} else {
				_syncState = 1;
			}
			break;

		case 0: // 1
		default:
			if (syncMark[_syncState] == i) {
				_syncState++;
			}
			break;
	}

	return ret;
}

