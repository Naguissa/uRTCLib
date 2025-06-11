/**
 * WARNING: This example is mostly the same as uRTCLib_example_full but adds web interface to ESP8266 and ESP32
 *
 * This example needs uEspConfigLib library, https://github.com/Naguissa/uEspConfigLib
 *
 * Connect to open WiFi called uRTCLib_device and browse to http://192.168.4.1/
 *
 *
 * Check "void ESPSetup()" function, "configFs" variable if you want to save WiFi data permanently
 *
 *
 * You can use this tool to set any RTC device.
 *
 */


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
 *     * Power lost flag reading and clearing
 *     * Enable Oscillator flag to check if Oscillator will run on VBAT
 *     * Set Clock in 12 hour or 24 hour mode. Get AM PM if in 12 hour mode. (Alarm set still in 24 hour mode)
 *
 * See uEEPROMLib for EEPROM support.
 *
 * @copyright Naguissa
 * @author Naguissa
 * @url https://github.com/Naguissa/uRTCLib
 * @url https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html
 * @email naguissa@foroelectro.net
 */
#include "Arduino.h"
#include "uRTCLib.h"



uRTCLib rtc;

// URTCLIB_MODEL_DS1307
// URTCLIB_MODEL_DS3231
// URTCLIB_MODEL_DS3232
byte rtcModel = URTCLIB_MODEL_DS3231;

uint8_t position;


// ESP8266 or ESP32 specific section:
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
    #include "uEspConfigLib.h"
    uEspConfigLibFSInterface * configFs;
    uEspConfigLib * config;
    bool restartWiFi = false;

    #ifdef ARDUINO_ARCH_ESP32
        WebServer server(80);
    #else
        ESP8266WebServer server(80);
    #endif

    void ESPSetup();
    void WiFiSetup();
    void handleGetConfigHtml();
    void handleSaveConfig();
    void handleDefault();
    void handleConfigRTC();
    void handleSaveConfigRTC();
    
    
    
    void ESPSetup() {
	    Serial.println("ESP configuration start");
	    
        configFs = new uEspConfigLibFSNone("/config.ini", true);
        // None (lost on reboot) configFs = new uEspConfigLibFSNone("/config.ini", true);
        // SPIFFS: configFs = new uEspConfigLibFSSpiffs("/config.ini", true);
        // LittleFS: configFs = new uEspConfigLibFSLittlefs("/config.ini", true);
        // SD-card: configFs = new uEspConfigLibFSSd("/config.ini", true);
        if (configFs->status() == uEspConfigLibFS_STATUS_FATAL) {
            Serial.println("  * Error initializing FS");
        }

        config = new uEspConfigLib(configFs);

        // Define your variables: Name, Description, Default value
        config->addOption("wifi_mode", "WiFi mode (C=Client, other=Access Point)", "A");
        config->addOption("wifi_ssid", "SSID of your WiFi", "uRTCLib_device");
        config->addOption("wifi_password", "Password of your WiFi", "");
    
        // Not useful on FS type none: config->loadConfigFile();
        
	    Serial.println("ESP configuration done");
    }
        
        
    
    void WiFiSetup() {
	    Serial.println("WiFi setup start");
        char *mode, *ssid, *pass;
        
	    server.stop();
	    WiFi.disconnect();
        #ifndef ARDUINO_ARCH_ESP32
	        WiFi.setAutoConnect(true);
	    #endif
	    WiFi.setAutoReconnect(true);
	    
        mode = config->getPointer("wifi_mode");
        ssid = config->getPointer("wifi_ssid");
        pass = config->getPointer("wifi_password");

        if (strcmp(mode, "S") == 0) {
		    WiFi.mode(WIFI_STA);
		    WiFi.begin(ssid, pass);
		    // Wait for connection
		    uint8_t i = 0;
		    while (WiFi.status() != WL_CONNECTED && i++ < 30) { //wait 30*2 seconds
			    Serial.print('.');
			    delay(2000);
		    }
		    if (i == 31) {
			    Serial.print("Could not connect to ");
			    Serial.println(ssid);
			    return;
		    }
		    Serial.print("Connected! IP address: ");
		    Serial.println(WiFi.localIP());
	    } else { // Default mode, 'A' (AP)
		    WiFi.mode(WIFI_AP);
		    WiFi.softAP(ssid, pass);
		    Serial.print("SoftAP created! IP address: ");
		    Serial.println(WiFi.softAPIP());
	    }

	    server.on("/config", HTTP_GET, handleGetConfigHtml);
	    server.on("/saveConfig", HTTP_POST, handleSaveConfig);
	    server.on("/configRTC", HTTP_GET, handleConfigRTC);
	    server.on("/saveConfigRTC", HTTP_POST, handleSaveConfigRTC);
	    
	    server.onNotFound(handleDefault);
        yield();
	    server.begin();
        yield();
	    Serial.println("HTTP server started");
	    Serial.println("WiFi setup done");
    }
    
    void handleGetConfigHtml() {
        Serial.println(" -> Requested configHTML");
        yield();
        config->handleConfigRequestHtml(&server, "/saveConfig");
    }
    void handleSaveConfig() {
        Serial.println(" -> Requested saveConfig");
        yield();
        config->handleSaveConfig(&server);
        restartWiFi = true;
    }
    void handleConfigRTC() {
        Serial.println(" -> Requested configRTC");
        
        server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        yield();
        server.send(200, "text/html", "");
        yield();
        server.sendContent("<html><head><title>uRTCLib ESP example</title></head><body>");
        yield();

        server.sendContent("<h1>uRTCLib ESP example</h1>");
        yield();

        server.sendContent("<p>RTC configuration:</p><form method=\"POST\" action=\"/saveConfigRTC\"><table style=\"border:1px solid #333\">");
        yield();

	    // We use global object, refreshed each second in loop function: rtc.refresh();
        server.sendContent("<tr><td>Year (2 digits):</td><td><input type=\"text\" name=\"year\" value=\"" + String(rtc.year(), DEC) + "\"></td></tr>");
        yield();

        server.sendContent("<tr><td>Month:</td><td><input type=\"text\" name=\"month\" value=\"" + String(rtc.month(), DEC) + "\"></td></tr>");
        yield();

        server.sendContent("<tr><td>Day:</td><td><input type=\"text\" name=\"day\" value=\"" + String(rtc.day(), DEC) + "\"></td></tr>");
        yield();

        uint8_t temp = rtc.hour();
        if(rtc.hourModeAndAmPm() == 2) temp += 12;
        server.sendContent("<tr><td>Hour (24h mode!):</td><td><input type=\"text\" name=\"hour\" value=\"" + String(temp, DEC) + "\"></td></tr>");
        yield();



        server.sendContent("<tr><td>Minute:</td><td><input type=\"text\" name=\"minute\" value=\"" + String(rtc.minute(), DEC) + "\"></td></tr>");
        yield();

        server.sendContent("<tr><td>Second:</td><td><input type=\"text\" name=\"second\" value=\"" + String(rtc.second(), DEC) + "\"></td></tr>");
        yield();

        server.sendContent("<tr><td>DayOfWeeK (1=Sunday):</td><td><input type=\"text\" name=\"dow\" value=\"" + String(rtc.dayOfWeek(), DEC) + "\"></td></tr>");
        yield();

        temp = (rtc.hourModeAndAmPm() == 0  ? 0 : 1);
        server.sendContent("<tr><td>12/24h mode (1=Sunday):</td><td><input type=\"radio\" name=\"mode12h\" value=\"0\"" + String(temp == 0 ? " checked=\"checked\"" : "")  + ">24h // <input type=\"radio\" name=\"mode12h\" value=\"1\"" + String(temp != 0 ? " checked=\"checked\"" : "")  + ">12h</td></tr>");
        yield();

        server.sendContent("<tr><td>Clear Lost power flag:</td><td><input type=\"checkbox\" name=\"lostPowerClear\" value=\"1\"></td></tr>");
        yield();

        server.sendContent("<tr><td>Aging:</td><td><input type=\"text\" name=\"aging\" value=\"" + String(rtc.agingGet(), DEC) + "\"></td></tr>");
        yield();
        
        server.sendContent("<tr><td>Enable OSCilator?:</td><td><input type=\"radio\" name=\"esoc\" value=\"0\"" + String(rtc.getEOSCFlag() ? "" : " checked=\"checked\"")  + ">Disabled // <input type=\"radio\" name=\"esoc\" value=\"1\"" + String(rtc.getEOSCFlag() ? " checked=\"checked\"" : "")  + ">Enabled</td></tr>");
        yield();

        server.sendContent("<tr><td colspan=\"2\"><center><button type=\"submit\">Save</button></center></td></tr></table></form></body>");
        yield();
    }
    
    void handleSaveConfigRTC() {
        Serial.println(" -> Requested saveConfigRTC");
        yield();

        rtc.set(atoi(server.arg("second").c_str()), atoi(server.arg("minute").c_str()), atoi(server.arg("hour").c_str()), atoi(server.arg("dow").c_str()), atoi(server.arg("day").c_str()), atoi(server.arg("month").c_str()), atoi(server.arg("year").c_str()));
        yield();
        Serial.println("RTC set.");

        rtc.set_12hour_mode(server.arg("mode12h") == "1");
        yield();
        Serial.println("set_12hour_mode.");
        
        if(server.arg("lostPowerClear") == "1") {
            rtc.lostPowerClear();
            yield();
            Serial.println("lostPowerClear.");
        }

        if(server.arg("esoc") == "0") {
            rtc.disableBattery();
            Serial.println("disableBattery.");
        } else {
            rtc.enableBattery();
            Serial.println("enableBattery.");
        }
        yield();
        
        rtc.agingSet(atoi(server.arg("aging").c_str()));
        yield();
        Serial.print("agingSet.");
        
        server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        server.sendHeader("Location", "/", true);
        server.send(302, "text/html","");
    }

    void handleDefault() {
        String line;
        Serial.println(" -> Requested default");
        server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        yield();
        server.send(200, "text/html", "");
        yield();
        server.sendContent("<html><head><title>uRTCLib ESP example</title></head><body>");
        yield();

        server.sendContent("<h1>uRTCLib ESP example</h1>");
        yield();

        server.sendContent("<p>Current global state:</p>");
        yield();

	    // We use global object, refreshed each second in loop function: rtc.refresh();

        line = "<p> - RTC DateTime (YY/MM/DD HH:MM:ss): " + String(rtc.year(), DEC) + "/" + String(rtc.month(), DEC) + "/" + String(rtc.day(), DEC) + " " + String(rtc.hour(), DEC) + ":" + String(rtc.minute(), DEC) + ":" + String(rtc.second(), DEC);
	    switch(rtc.hourModeAndAmPm()) {
	        case 1: line += " AM"; break;
	        case 2: line += " PM"; break;
	        case 0:
	        default: line += " (24h mode)"; break;
        }
        
	    if(rtc.hourModeAndAmPm() == 1) line += " AM";
	    if(rtc.hourModeAndAmPm() == 2) line += " PM";
        line += " DOW: " + String(rtc.dayOfWeek(), DEC) + " - Temp: " + String(((float) rtc.temp() / 100), 2) + "<br>";
        yield();
        server.sendContent(line);
        yield();

	    // Check whether EOSC is set to use VBAT or not
	    if (rtc.getEOSCFlag()) server.sendContent(" - Oscillator will not use VBAT when VCC cuts off. Time will not increment without VCC!<br>");
	    else server.sendContent(" - Oscillator will use VBAT when VCC cuts off.<br>");
        yield();

	    if (rtc.lostPower()) {
		    server.sendContent(" - Lost power status: POWER FAILED.<br>");
	    } else {
		    server.sendContent(" - Lost power status: POWER OK.<br>");
	    }
        yield();

	    server.sendContent(" - Aging register value: " + String(rtc.agingGet(), DEC) + "</p>");
        yield();
        
        server.sendContent("<p>Available items:</p>");
        yield();

        server.sendContent("<p><a href=\"/config\">ESP WiFi configuration</a></p>");
        yield();
        server.sendContent("<p><a href=\"/configRTC\">RTC configuration</a></p>");
        yield();
            
        server.sendContent("</body>");
    }
    
#endif




void setup() {
	delay (2000);
	Serial.begin(115200);
	Serial.println("Serial OK");
	//  Max position: 32767

	#ifdef ARDUINO_ARCH_ESP8266
		URTCLIB_WIRE.begin(0, 2); // D3 and D4 on ESP8266
	#else
		URTCLIB_WIRE.begin();
	#endif
	rtc.set_rtc_address(0x68);

	// set RTC Model
	rtc.set_model(rtcModel);


    #if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
        ESPSetup();
        WiFiSetup();
    #endif


	// refresh data from RTC HW in RTC class object so flags like rtc.lostPower(), rtc.getEOSCFlag(), etc, can get populated
	rtc.refresh();

	// Only use once, then disable
	//rtc.set(0, 42, 16, 6, 2, 5, 15);
	//  RTCLib::set(byte second, byte minute, byte hour (0-23:24-hr mode only), byte dayOfWeek (Sun = 1, Sat = 7), byte dayOfMonth (1-12), byte month, byte year)

	// use the following if you want to set main clock in 12 hour mode
	// rtc.set_12hour_mode(true);

	// use the following if you want to set main clock in 24 hour mode
	// rtc.set_12hour_mode(false);


	// Check whether OSC is set to use VBAT or not
	if (rtc.getEOSCFlag())
		Serial.println(F("Oscillator will not use VBAT when VCC cuts off. Time will not increment without VCC!"));
	else
		Serial.println(F("Oscillator will use VBAT when VCC cuts off."));

	Serial.print("Lost power status: ");
	if (rtc.lostPower()) {
		Serial.print("POWER FAILED. NOT Clearing flag in this example...");
		// rtc.lostPowerClear();
		Serial.println(" done.");
	} else {
		Serial.println("POWER OK");
	}


	Serial.print("Aging register value: ");
	Serial.println(rtc.agingGet());

	Serial.println("Not changing aging register value. To do so you can execute: rtc.agingSet(newValue)");



	// Set SRAM (DS3231 doen't have SRAM, so it will not store anything and will return always 0xff)
	for (position = 0; position < 255; position++) {
        #if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
	        server.handleClient();
            if (restartWiFi) {
                restartWiFi = false;    
                WiFiSetup();
            }
        #endif
		rtc.ramWrite(position, position);
	}

	if(rtcModel == URTCLIB_MODEL_DS3231 || rtcModel == URTCLIB_MODEL_DS3232) {
		rtc.alarmClearFlag(URTCLIB_ALARM_1);
		rtc.alarmClearFlag(URTCLIB_ALARM_2);

		// Alarm pin will get low at sny :10 seconds. We clear the flag on loop to be able to see it, as it doesn't clear itself when triggered
		rtc.alarmSet(URTCLIB_ALARM_TYPE_1_FIXED_S, 10, 0, 0, 1); // Each minute, at just :10 seconds
		// RTCLib::alarmSet(uint8_t type, uint8_t second, uint8_t minute, uint8_t hour, uint8_t day_dow);
	}

	position = 0;

}

void loop() {
    #if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
	    server.handleClient();
        if (restartWiFi) {
            restartWiFi = false;    
            WiFiSetup();
        }
    #endif
	rtc.refresh();
	Serial.print("RTC DateTime (YY/MM/DD HH:MM:ss): ");

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
	switch(rtc.hourModeAndAmPm()) {
	    case 1: Serial.print(" AM"); break;
	    case 2: Serial.print(" PM"); break;
	    case 0:
	    default: Serial.print(" (24h mode)"); break;
    }

	Serial.print(" DOW: ");
	Serial.print(rtc.dayOfWeek());

	Serial.print(" - Temp: ");
	Serial.print((float) rtc.temp() / 100);

	if(rtcModel == URTCLIB_MODEL_DS1307 || rtcModel == URTCLIB_MODEL_DS3232) {
		Serial.print(" - SRAM position ");
		Serial.print(position);
		Serial.print(" value: ");
		Serial.print(rtc.ramRead(position), HEX);
		position++;
	}

	Serial.println();

	if(rtcModel == URTCLIB_MODEL_DS3231 || rtcModel == URTCLIB_MODEL_DS3232) {
		rtc.alarmClearFlag(URTCLIB_ALARM_1);
		rtc.alarmClearFlag(URTCLIB_ALARM_2);
	}

	delay(1000);
}
