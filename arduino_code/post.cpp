#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

const char * ssid = "homewifi_D96";
const char * password = "16706115";

//Your Domain name with URL path or IP address with path
const char * serverName = "http://192.168.0.14:8000/records/";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
	Serial.begin(115200);

	WiFi.begin(ssid, password);
	Serial.println("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to WiFi network with IP Address: ");
	Serial.println(WiFi.localIP());

	Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
	if (WiFi.status() == WL_CONNECTED) {
		WiFiClient client;
		HTTPClient http;

		// Your Domain name with URL path or IP address with path
		http.begin(client, serverName);

		// Specify content-type header
		// http.addHeader("Content-Type", "application/x-www-form-urlencoded");
		// Data to send with HTTP POST
		// String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";           
		String httpRequestData = "{\"prototype_id\": 1,\"ph\": 2.12, \"temperature\": 20.40, \"humidity\": 0.90}";
		// "{\"prototype_id\": 1,\"ph\": 2.12, \"temperature\": 20.40, \"humidity\": 0.90}";
		// Send HTTP POST request
		http.addHeader("Content-Type", "application/json");
		int httpResponseCode = http.POST(httpRequestData);

		// If you need an HTTP request with a content type: application/json, use the following:
		//http.addHeader("Content-Type", "application/json");
		//int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

		// If you need an HTTP request with a content type: text/plain
		//http.addHeader("Content-Type", "text/plain");
		//int httpResponseCode = http.POST("Hello, World!");

		Serial.print("HTTP Response code: ");
		Serial.println(httpResponseCode);

		// Free resources
		http.end();
	} else {
		Serial.println("WiFi Disconnected");
	}
	// lastTime = millis();
	delay(3000);

}