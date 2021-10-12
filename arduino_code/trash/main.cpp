/*
	Mateus Victor - 27/06/2021
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// WiFi variables
const char* ssid = "USER_SSID"; 
const char* password = "USER_PASSWORD";

// Host name and API endpoint URL
const char* url = "http://mateusinho/sensors/72h132-3jkad327-1238kjsdf9781-adfkn18u2";

// Time delay variables
unsigned long last_time = 0;
unsigned long timer_delay = 5000;

String sensor_readings;
float sensor_readings_arr[3];

void setup()
{
	Serial.begin(115200);

	WiFi.begin(ssid, password);
	Serial.println("Connecting to WiFi...");

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("Connected to WiFi network with IP Adress: ");
	Serial.println(WiFi.localIP());

	Serial.println("Please wait...")
}


void loop()
{
	if ((millis() - last_time) > timer_delay)
	{	
		if (WiFi.status() == WL_CONNECTED)
		{
			sensor_readings = get_request(url);
			Serial.println(sensor_readings);

			JSONVar json_payload = JSON.parse(sensor_readings);

			if (JSON.typeof(json_payload) == "undefined")
			{
				Serial.println("Parsing input failed!");
				return 
			}

			print_json_response(json_payload);
		}
		else
		{
			Serial.println("WiFi disconnected!");
		}
		last_time = millis();
	}
}

void print_json_response(JSONVar json_payload)
{
	JSONVar keys = json_payload.keys();
	Serial.println("{");

	for (int i=0; i<keys.length(); i++)
	{
		JSONVar value = json_payload[keys[i]];
		Serial.println("   " + keys[i] + ": " + "\"" + value + "\"");
	}

	Serial.println("}");
}

String get_request(const char* url)
{
	HTTPClient http;
	http.begin(server_name);

	// Send HTTP GET request
	int response_code = http.GET();

	String payload = "{}"; 

	if (response_code>0)
	{
		Serial.print("HTTP Response code: ");
		Serial.println(response_code);
		payload = http.getString();
	}
	else {
		Serial.print("Error code: ");
		Serial.println(response_code);
	}

	http.end();

	return payload;
}