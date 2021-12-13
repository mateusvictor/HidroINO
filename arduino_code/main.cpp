/*
  Mateus Victor
  GitHub: @mateusvictor
  Email: mateus_victors@outlook.com
  São Paulo, Brazil - 2021
*/

#include "WiFiEsp.h"
#include "DHT.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

// Sensors - Inputs
#define PH_PIN A0
#define DHT_PIN 8
#define DHTTYPE DHT11

// Outputs
#define SOL1_PIN 12

DHT dht(DHT_PIN, DHTTYPE); // DHT object to read humidity and temperature values
WiFiEspClient client; // Ethernet client object to send HTTP requests

/* 
--------------------------- WiFi connection and related variables ------------------------
*/
unsigned long lastPost = 0; // Note the time of the last POST request made
const unsigned long postingInterval = 3000; // Interval between requests

// WiFi credentials and prototype ID
char ssid[] = "hidro";
char pass[] = "12345678";

// Individual prototype ID
String prototypeId = "9d0efcbb-1c6f-4486-8066-19c9cb2352fd";

int status = WL_IDLE_STATUS;
char server[] = "hidroino.herokuapp.com";
int port = 80;

/* 
--------------------------- Sensors parameters and devices states/data ------------------------
*/
// pH reference value to open/close solenoid
const unsigned int pHReference = 7.5;
float calibration = 23.60;

// Devices states
int Solenoid1 = 0, Solenoid2 = 0, Led1 = 0;
unsigned long counter = 1;

// Data
String sensorsData;


void setup()
{
  Serial.begin(9600);

  // ESP8266 Module inicialization
  Serial1.begin(9600);
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  // Attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();

  pinMode(SOL1_PIN, OUTPUT); 
  digitalWrite(SOL1_PIN, 0);
}


void loop()
{
  // If the difference between the current time and the last request 
  // is greater than the POST interval, make the HTTP POST request.
  if (millis() - lastPost > postingInterval){
    sensorsData = getSensorValues();
    httpPOSTRequest(sensorsData);
  }
}


String getSensorValues()
{
  float ph = getPH(); // random(2, 12);
  checkSolenoid(ph);

  float temp = dht.readTemperature(); // random(5, 35);
  int humidity = dht.readHumidity(); // random(5,95);
  
  String data = "{\"prototype_id\": \"" + String(prototypeId) + 
      "\",\"ph\": " + String(ph) + 
      ",\"temperature\": " +  String(temp) + 
      ", \"humidity\": " + String(humidity) + 
      ", \"device1\": " + String(Led1) + 
      ", \"device2\": " + String(Solenoid1) + 
      ", \"device3\": " + String(Solenoid2) + "}";
      
  return data;
}


void httpPOSTRequest(String content)
{
  // STOP the client from the last request
  client.stop();

  Serial.println();
  Serial.print("( POST ) Request number: ");
  Serial.println(counter);

  Serial.print("Data: ");
  Serial.println(content);

  if (client.connect(server, port)) {
    Serial.println("Connected to server");

    client.println("POST /records/ HTTP/1.1");
    client.println("Host: hidroino.herokuapp.com");
    client.println("Content-Length: " + String(content.length()));
    client.println("Content-Type: application/json");
    client.println();
    client.println(content);

    // Note the time that the connection was made
    lastPost = millis();
  }
  else {
    Serial.println("Connection failed");
  }

  Serial.println();
  counter += 1;
}


void checkSolenoid(float ph)
{
  if (ph > pHReference){
    digitalWrite(SOL1_PIN, 1);
    Solenoid1 = 1;
  }
  else {
    digitalWrite(SOL1_PIN, 0);
    Solenoid1 = 0;
  }
}


float getPH()
{
  int buf[10], temp;
  unsigned long int avgValue;
  float pHVol, pHValue;

  // Reading the values
  for(int i=0;i<10;i++)
  {
    buf[i]=analogRead(PH_PIN);
    delay(30);
  }

  // Sorting the values in ascending order
  for(int i=0;i<9;i++)
    {
    for(int j=i+1;j<10;j++)
    {
    if(buf[i]>buf[j])
    {
      temp=buf[i];
      buf[i]=buf[j];
      buf[j]=temp;
      }
    }
  }

  avgValue=0;
  for(int i=2;i<8;i++)
    avgValue+=buf[i];

  pHVol=(float)avgValue*5.0/1024/6;
  pHValue = -5.70 * pHVol + calibration;

  return pHValue;
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}