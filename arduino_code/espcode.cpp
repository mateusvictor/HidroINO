// 
// ARDUINO --> STRING --> ESP8266 Module
// 
#include <SoftwareSerial.h>

SoftwareSerial myserial(0, 1);
// SoftwareSerial myserial(2, 3);

void setup()
{
	Serial.begin(9600);
	myserial.begin(9600);
	delay(5000);
}

void loop()
{
	String inputString = "";

	while (myserial.avaliable()){
		inputString = myserial.readString();
		Serial.println("Received String: " + inputString);
	}
}
