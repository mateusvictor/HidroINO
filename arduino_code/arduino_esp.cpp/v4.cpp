#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "homewifi_D96";            
char pass[] = "16706115";        
int status = WL_IDLE_STATUS;     

char server[] = "192.168.0.14";

unsigned long lastConnectionTime = 0;         
const unsigned long postingInterval = 5000; 

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);

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
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
//  while (client.available()) {
 //   char c = client.read();
  //  Serial.write(c);
  //}

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    httpPOSTRequest(getSensorValues());
  }
  
}

String getSensorValues(){
  float ph = random(2, 12);
  float temp = random(5, 35);
  int humidity = random(5,95);
  String data = "{\"prototype_id\": \"6e647f14-cff0-4205-bafc-c26e0e3deaa8\",\"ph\": " + String(ph) + ",\"temperature\": " +  String(temp) + ", \"humidity\": " + String(humidity) + "}";
  return data;
}

// this method makes a HTTP connection to the server
void httpPOSTRequest(String content)
{
  Serial.println(content);
  Serial.println();

  
  WiFiEspClient client;
  delay(1000);

  if (client.connect(server, 8000)) {
    Serial.println("Connected to server");

    client.println("POST /records/ HTTP/1.0");
    client.println("Host: 192.168.0.14:8000");
    //client.println("Accept: */*");
    client.println("Content-Length: " + String(content.length()));
    client.println("Content-Type: application/json");
    client.println();
    client.println(content);

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection 
    Serial.println("Connection failed");
  }
  client.stop();
  delay(1000);
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