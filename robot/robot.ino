#include <vector>
using namespace std; // so we can just type vector instead of std::vector
/*
* Original PCA9685 Module library sourse: https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
 * 
 * This is the Arduino code PCA6985 32 channel servo controller
 * to control 32 Servo Motors over WiFi using ESP32 MCU
 * 
 * Watch video for this code: 
 * 
 * Related Videos
V5 video of PCA9685 32 Servo with ESP32 with WiFi https://youtu.be/bvqfv-FrrLM
V4 video of PCA9685 32 Servo with ESP32 (no WiFi): https://youtu.be/JFdXB8Za5Os
V3 video of PCA9685 how to control 32 Servo motors https://youtu.be/6P21wG7N6t4
V2 Video of PCA9685 3 different ways to control Servo motors: https://youtu.be/bal2STaoQ1M
V1 Video introduction to PCA9685 to control 16 Servo	https://youtu.be/y8X9X10Tn1k
 *	
 * Written by Ahmad Shamshiri for Robojax Video channel www.Robojax.com
 * Date: Jan 25, 2021, in Ajax, Ontario, Canada

 * this code has been downloaded from http://robojax.com/learn/arduino/
 
 * Get this code and other Arduino codes from Robojax.com
Learn Arduino step by step in structured course with all material, wiring diagram and library
all in once place. Purchase My course on Udemy.com http://robojax.com/L/?id=62

****************************
Get early access to my videos via Patreon and have	your name mentioned at end of very 
videos I publish on YouTube here: http://robojax.com/L/?id=63 (watch until end of this video to list of my Patrons)
****************************

or make donation using PayPal http://robojax.com/L/?id=64

 *	* This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program.	If not, see <https://www.gnu.org/licenses/>.
 */
////////////////////// PCA9685 settings started

// #include <Wire.h>

#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver board2 = Adafruit_PWMServoDriver(0x41);
int maximumServo = 32; // how many servos are connected

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// Watch video V1 to understand the two lines below: http://youtu.be/y8X9X10Tn1k
#define SERVOMIN	125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX	575 // this is the 'maximum' pulse length count (out of 4096)

int servoAngle = 0;
int servoStep = 10;

int stepDelay = 50; // 50 milliseconds
int servoAngleMin = 0;
int servoAngleMax = 180;

// minimum angle of each servo		 
bool servoShown[] = {
	0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 0, 0, 0, 0, 0, 0,
};

int allServoMin[] = {
	0, 0, 0, 0, 0, 0, 0, 0, //  0 to  7
	0, 0, 0, 0, 0, 0, 0, 0, //  8 to 15
	0, 0, 0, 0, 0, 0, 0, 0, // 16 to 23
	0, 0, 0, 0, 0, 0, 0, 0	// 24 to 31	
}; 
//maximum value of each servo

int allServoMax[] = {
	180, 180, 180, 180, 180, 180, 180, 180,	//  0 to  7
	180, 180, 180, 180, 180, 180, 180, 180,	//	8 to 15
	180, 180, 180, 180, 180, 180, 180, 180,	// 16 to 23 
	180, 180, 180, 180, 180, 180, 180, 180	// 24 to 31 
}; 
// initial position of servos			 
int allServoPosition[] = {
	 90,  98,  52, 119,  80, 110, 123, 100, //  0 to  7
	112, 122,  90,  90,  90,  90,  90,  90, //	8 to 15
	 93,  88,  68,  95,  89,  61, 100,  87, // 16 to 23
	 98,  68,  90,  90,  90,  90,  90,  90  // 24 to 31								
};
int servoNumber = 100; //servo to move
int buttonPushed = 0, batchMove = 0;
int allServo = 0;

void handleServo();//this is prototype of function defined at the end of this code
int angleToPulse(int ang); //this is prototype of function defined at the end of this code
////////////////////////PCA9685 ended

#include "PCA9684_32Servo_ESP32.h"
	
#include <WiFi.h>
#include <WiFiMulti.h>
// #include <WiFiClient.h>
// ref: https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/wifi.html

WiFiMulti wifiMulti;

#include <WebServer.h>
#include <ESPmDNS.h>

const int wifiApMode = 1;
// Station mode / STA: a wifi client device
// Access Point mode / AP: a wifi hotspot

/*for wifi AP mode*/
const char *apSsid = "6_legs_robot",
           *apPassword = "6_legs_robot";
IPAddress apLocalIp(192, 168, 54, 87), apGateway(192, 168, 54, 0), apSubnetMask(255, 255, 255, 0);

/*for wifi client mode*/
const char *ssid = "Android xxa5",
           *password = "beb5cb590c36";
const char *ssid2 = "toshi",
           *password2 = "1357924680";

WebServer server(80);

const int led = 13;

void handleInfo() {
	String minStr = "", maxStr = "", shownStr = "", posStr;
		// turn arrays allServoMin, allServoMax, servoShown, and allServoPosition into string

	if(maximumServo > 0){ // the first is special: `${thing}`
		minStr.concat(allServoMin[0]);
		maxStr.concat(allServoMax[0]);
		shownStr.concat(servoShown[0] ? "true" : "false");
		posStr.concat(allServoPosition[0]);
	}
	for(int i = 1; i < maximumServo; i++){ // others are in this pattern: `, ${thing}`
		minStr.concat(",");
		maxStr.concat(",");
		shownStr.concat(",");
		posStr.concat(",");

		minStr.concat(allServoMin[i]);
		maxStr.concat(allServoMax[i]);
		shownStr.concat(servoShown[i] ? "true" : "false");
		posStr.concat(allServoPosition[i]);
	}

	char info[minStr.length() + maxStr.length() + shownStr.length() + 200] = "";
	sprintf(info, R"json({
		"success": true,
		"data": {
			"num": %d,
			"runningAll": %s,
			"min": [%s],
			"max": [%s],
			"shown": [%s],
			"pos": [%s]
		}
	})json", maximumServo, allServo ? "true": "false", minStr.c_str(), maxStr.c_str(), shownStr.c_str(), posStr.c_str());
	server.send(200, "application/json", info);
}

void handleNotFound() {
	digitalWrite(led, 1);

	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}

	server.send(404, "text/plain", message);
	digitalWrite(led, 0);
}


void setup() {
	//Watch video explanation for this https://youtu.be/bvqfv-FrrLM
	board1.begin();
	board2.begin();  
	board1.setPWMFreq(60);	// Analog servos run at ~60 Hz updates
	board2.setPWMFreq(60);

	//initial position of all servos
	for(int i = 0; i < maximumServo; i++) {
		if(i < 16) {
			board1.setPWM(i		, 0, angleToPulse(allServoPosition[i]) ); 
		} else {
			board2.setPWM(i-15, 0, angleToPulse(allServoPosition[i]) );				
		}
	}

	Serial.begin(115200);
	delay(10);
	Serial.println("32 channel Servo test!");

	if(wifiApMode){
		WiFi.softAPConfig(apLocalIp, apGateway, apSubnetMask);
		while(true){
			if(!WiFi.softAP(apSsid, apPassword)){
				Serial.println("AP creation failed. Retrying...");
				delay(500);
			}else{
				break;
			}
		}

		Serial.print("AP created.\n IP:");
		Serial.println(WiFi.softAPIP());
	}else{
		wifiMulti.addAP(ssid, password);
		wifiMulti.addAP(ssid2, password2);

		Serial.println("Connecting Wifi");
			
		// Wait for connection
		while (wifiMulti.run() != WL_CONNECTED) { 
			delay(500);
			Serial.print(".");
		}
		
		Serial.println("");
		Serial.print("Connected to ");
		Serial.println(WiFi.SSID());
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
	}



	if (MDNS.begin("robojaxESP32")) {
		Serial.print("MDNS responder started at http://");
		Serial.println("robojaxESP32");
	}

	server.enableCORS();
	server.on("/", [](){ server.send(200, "text/pure", "Please open index.html in your browser"); });
	server.on("/robot", [](){ server.send(200, "text/html", "6_legs_robot"); });
	server.on("/servo", HTTP_GET, handleServo); 
	server.on("/servos", HTTP_GET, handleServos);
	server.on("/info", handleInfo);

	server.onNotFound(handleNotFound);
	server.begin();
	Serial.println("HTTP server started");	
}

void loop() {
	server.handleClient();
	if(allServo){
		for(int angle = servoAngleMin; angle <= servoAngleMax; angle += servoStep){
			for(int i = 0; i < 16; i++) {			 
				board2.setPWM(i, 0, angleToPulse(angle));
				board1.setPWM(i, 0, angleToPulse(angle));
			}
			delay(stepDelay);
		}
		// robojax PCA9865 32 channel Servo control
		delay(100);				 
	}
	
	// if pushed		
	if(buttonPushed && 0 <= servoNumber && servoNumber < maximumServo){
		if(servoNumber < 16) {
			board1.setPWM( servoNumber	 , 0, angleToPulse(allServoPosition[servoNumber]) ); 
		} else {
			board2.setPWM( servoNumber-15, 0, angleToPulse(allServoPosition[servoNumber]) );			 
		}
	}
	buttonPushed = 0; 

	if(batchMove){
		for(int i = 0; i < maximumServo; i++) {
			if(i < 16) {
				board1.setPWM(i   , 0, angleToPulse(allServoPosition[i]) ); 
			} else {
				board2.setPWM(i-15, 0, angleToPulse(allServoPosition[i]) );				
			}
		}
	}
	batchMove = 0;
}



/*
 * handleServo()
 * update the buttonPushed varialbe
 * returns nothing
 * Written by Ahmad Shamshiri on Dec 29, 2019
 * www.Robojax.com
 * http://youTube.com/robojaxTV
 */
void handleServo() {
	// Watch video explanation for this https://youtu.be/bvqfv-FrrLM
	allServo = server.arg("do") == "all" ? 1 : 0;

	int argServo = server.arg("servo").toInt(),
	    argDeg = server.arg("deg").toInt();
	if(
		0 <= argServo && argServo < maximumServo &&
		allServoMin[argServo] <= argDeg && argDeg <= allServoMax[argServo]
	) {
		allServoPosition[argServo] = argDeg; 
		servoNumber = argServo;

		buttonPushed = 1;
	}
	// handleRoot();
	server.send(200, "text/plain", "Finished.");
}

vector<String> splitString(String source, String splitter = ""){
	const int splLen = splitter.length(),
						allLen = source.length();

	vector<String> result;
	int indexL = 0, indexR = 0;

	while(indexL < allLen){
		indexR = source.indexOf(splitter, indexL);
		if(indexR == -1) indexR = allLen;
		if(splLen == 0) indexR += 1;

		result.push_back(source.substring(indexL, indexR));

		indexL = indexR + splLen;
	}

	return result;
}

void handleServos(){
	vector<int> servos, degs;

	{
		vector<String> servos_str = splitString(server.arg("servos"), ","),
		               degs_str = splitString(server.arg("degs"), ",");
		for(String str : servos_str){
			servos.push_back(str.toInt());
		}
		for(String str : degs_str){
			degs.push_back(str.toInt());
		}
	}

	if(servos.size() == 0 || degs.size() == 0)
		return server.send(400, "text/plain", "Both the \"servos\" and \"degs\" request arguments that are separated by a comma cannot have a length of 0.");

	for(int i = 0; i < servos.size(); i++){
		if( !(0 < servos[i] && servos[i] < maximumServo && servoShown[servos[i]]) )
			continue;

		allServoPosition[servos[i]] = degs[i < degs.size() ? i : degs.size() - 1];
	}

	batchMove = 1;
	server.send(200, "text/plain", "Finished");
}

/*
 * angleToPulse(int ang)
 * gets angle in degree and returns the pulse width
 * also prints the value on seial monitor
 * written by Ahmad Shamshiri for Robojax, Robojax.com
 */
int angleToPulse(int ang) {
	 int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max
	 return pulse;
}
