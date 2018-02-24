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
#ifndef _UMFMLIB_
	#define _UMFMLIB_
	#include "Arduino.h"


	#define UMFMFLIB_TYPE_FM 0
	#define UMFMFLIB_TYPE_MFM 1


	#define UMFMFLIB_LASTTYPE_DATA true
	#define UMFMFLIB_LASTTYPE_CLOCK false

	class uMFMLib {
		public:
			// Constructors
			uMFMLib();
			void reset();
			void setType(unsigned char);

			unsigned char encode(bool);
			unsigned char decode(bool);

			const bool syncMark[15] = {1,0,0,0,1,0,0,1,0,0,0,1,0,0,1}; // 1st bit is missing

			bool waitForSync(bool);

		private:
			// Adresses
			bool _last;
			bool _lastType;
			unsigned char _type = UMFMFLIB_TYPE_MFM;

			unsigned char _syncState;




	};


#endif


