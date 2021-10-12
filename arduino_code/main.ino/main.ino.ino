// 
// ARDUINO --> STRING --> ESP8266 Module
// 
#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char *ssid = "homewifi_D96";
const char *password = "16706115";

//Your Domain name with URL path or IP address with path
const char * serverName = "http://192.168.0.14:8000/records/";

SoftwareSerial myserial(0, 1);
// SoftwareSerial myserial(2, 3);

void setup()
{
  Serial.begin(9600);
  myserial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  delay(5000);
}

void loop()
{
  String inputString = "";
  int httpResponseCode;

  while (myserial.available()){
    inputString = myserial.readString();
    Serial.println("Received String: " + inputString);

    httpResponseCode = sendPostRequest(inputString);

    if (httpResponseCode == 0)
    {
      Serial.println("WiFi Disconnected");
    } 
    else 
    {
      Serial.print("HTTP response code: ");
      Serial.println(httpResponseCode);
    }
  }
}

int sendPostRequest(String jsonData)
{
  if (WiFi.status() != WL_CONNECTED)
    return 0;

  WiFiClient client;
  HTTPClient http;
  int httpResponseCode;

  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/json");
  httpResponseCode = http.POST(jsonData);

  return httpResponseCode;
}
