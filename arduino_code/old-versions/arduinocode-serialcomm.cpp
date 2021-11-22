// 
// ARDUINO --> STRING --> ESP8266 Module
// 
void setup()
{
	Serial.begin(9600);
}
// 
// "{\"prototype_id\": 1,\"ph\": 2.12, \"temperature\": 20.40, \"humidity\": 0.90}"
//
void loop()
{
	String jsonData = "{
		\"prototype_id\": 1,
		\"ph\": 2.12, 
		\"temperature\": 20.40, 
		\"humidity\": 0.90
	}";

	Serial.write(jsonData)
	delay(2000);
}
