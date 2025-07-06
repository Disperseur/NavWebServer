#include "NMEA.h"
#include "server.h"


void sendHTML(WiFiClient &client);
void sendJSON(WiFiClient &client);
void sendNotFound(WiFiClient &client);

Nmea bateau;
String nmea_msg("");

//wifi credentials & variables
char ssid[] = "Saint-Lou_Wifi";  // your network SSID (name)
char pass[] = "123456789"; 
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(460800); // debug com
  Serial1.begin(460800); // data serial port on RX0


  Serial.println("Demarrage point d'acces : " + String(ssid));
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }
    
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true)
      ;
  }

  // print wifi status:
  Serial.println("SSID: " + String(WiFi.SSID()));
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}





void loop() {
  // parse new nmea msg
  while(Serial1.available()) {
    nmea_msg = Serial1.readStringUntil('\n');
    bateau.parse(nmea_msg);
  }
  // bateau.printData();



  WiFiClient client = server.available();  // listen for incoming clients

  if (client) {
    Serial.println("Client connected.");

    String request = client.readStringUntil('\r');
    client.read(); // consoomer '\n'
    Serial.println(request);

    if (request.indexOf("GET / ") >= 0) {
      sendHTML(client);
    } else if (request.indexOf("GET /data") >= 0) {
      sendJSON(bateau, client);
    } else {
      sendNotFound(client);
    }

    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }

  delay(100);
}