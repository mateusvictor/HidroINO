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

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 3000; // delay between updates, in milliseconds

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);
  pinMode(LED1_PIN, OUTPUT);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
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
  //DynamicJsonDocument doc(1024);
  String response;
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    response = httpGETRequest();
    if (response.length() > 5){
        float ph = 123;
        Serial.println(ph);
        Led1 = getValueFromJson(response, "device1");
        digitalWrite(LED1_PIN, Led1);
    }
  }

  
}

int getValueFromJson(String data, String key){
  int keyIndex = data.indexOf(key);
  Serial.println(data.charAt(keyIndex+9));
  if (data.charAt(keyIndex+9) == 't'){
    return 1;
  }
  else {
    return 0;
  }
}

// this method makes a HTTP connection to the server
String httpGETRequest()
{
  String response;
  int lines = 1;
  Serial.println();
    
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    
    // send the HTTP GET request
    client.println("GET /record-last/" + prototypeId +  "/ HTTP/1.1");
    client.println("Host: hidroino.herokuapp.com");
    client.println("Accept: application/json");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }

  // Get the JSON part of the response
  while (client.available()) {
    char c = client.read();
    if (lines == 9){
      response += c;
    }
    else {
      if (c == '\n'){
        lines += 1;
      }
    }
  }

  delay(1000);
  Serial.println(response);
  return response;
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