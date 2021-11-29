#include "WiFiEsp.h"

#define LED1_PIN 8
#define SOL1_PIN 12
#define SOL2_PIN 13

// {"prototype_id":"9d0efcbb-1c6f-4486-8066-19c9cb2352fd","ph":7.1,"temperature":27.0,"humidity":27.0,"device1":false,"device2":false,"device3":false,"datetime_creation":"26-11-2021 18:15:17","id":1}
// USAR INDEXOF!!!

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "homewifi_D96";            // your network SSID (name)
char pass[] = "16706115";  
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "hidroino.herokuapp.com";
String prototypeId = "9d0efcbb-1c6f-4486-8066-19c9cb2352fd";

// Devices states
int Solenoide1 = 0, Solenoide2 = 0, Led1 = 0;

unsigned long lastPost = 0;
const unsigned long postingInterval = 5000;

unsigned long counter = 1;
int port = 80;

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  Serial.begin(9600);

  // ESP8266 Module inicialization
  Serial1.begin(9600);
  WiFi.init(&Serial1);

  // Ports
  pinMode(LED1_PIN, INPUT);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  
  printWifiStatus();
}

void loop()
{
  String response;

  if (millis() - lastPost > postingInterval){
      httpPOSTRequest(getSensorValues());
  }
  
}


void httpPOSTRequest(String content)
{
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
    //client.println("Accept: */*");
    client.println("Content-Length: " + String(content.length()));
    client.println("Content-Type: application/json");
    client.println();
    client.println(content);

    // note the time that the connection was made
    lastPost = millis();
  }
  else {
    // if you couldn't make a connection 
    Serial.println("Connection failed");
  }
  Serial.println();
  counter += 1;
}

String getSensorValues(){
  float ph = random(2, 12);
  float temp = random(5, 35);
  int humidity = random(5,95);
  
  String data = "{\"prototype_id\": \"" + String(prototypeId) + 
      "\",\"ph\": " + String(ph) + 
      ",\"temperature\": " +  String(temp) + 
      ", \"humidity\": " + String(humidity) + 
      ", \"device1\": " + String(Led1) + 
      ", \"device2\": " + String(Solenoide1) + 
      ", \"device3\": " + String(Solenoide2) + "}";
      
  return data;
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