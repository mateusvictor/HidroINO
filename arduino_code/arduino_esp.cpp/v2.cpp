#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "homewifi_D96";            // your network SSID (name)
char pass[] = "16706115";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "192.168.0.14";


void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
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

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  //printWifiStatus();

}

void loop()
{
  // Initialize the Ethernet client object
  WiFiEspClient client;
  Serial.println();
  Serial.println("Starting connection to server...");

  // if you get a connection, report back via serial
  if (client.connect(server, 8000)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    String content = "{\"prototype_id\": 1,\"ph\": 2.12, \"temperature\": 20.40, \"humidity\": 0.90}";
    client.println("POST /records/ HTTP/1.1");
    client.println("Host: 192.168.0.14:8000");
    client.println("Accept: */*");
    client.println("Content-Length: " + String(content.length()));
    client.println("Content-Type: application/json");
    client.println();
    client.println(content);
    //client.println("Connection: close");
    while (client.available()) {
      char c = client.read();
     Serial.write(c);
    }

  }



  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();
  }

  delay(5000);
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