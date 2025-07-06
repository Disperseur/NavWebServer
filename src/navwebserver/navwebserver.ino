#include "NMEA.h"

#include <SPI.h>
#include <WiFi.h>
#include "page1.h"

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
      sendJSON(client);
    } else {
      sendNotFound(client);
    }

    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }

  delay(100);
}





void sendHTML(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.print(INDEX_HTML);
}

void sendJSON(WiFiClient &client) {
  String json = "{";
  json += "\"running_time\":" + String(bateau.get_running_time()) + ",";
  json += "\"ground_time\":\"" + bateau.get_ground_time() + "\",";
  json += "\"ground_date\":\"" + bateau.get_ground_date() + "\",";
  json += "\"ground_latitude\":\"" + bateau.get_ground_latitude() + "\",";
  json += "\"ground_latDir\":\"" + String(bateau.get_ground_latDir()) + "\",";
  json += "\"ground_longitude\":\"" + bateau.get_ground_longitude() + "\",";
  json += "\"ground_longDir\":\"" + String(bateau.get_ground_longDir()) + "\",";
  json += "\"ground_speedKts\":" + String(bateau.get_ground_speedKts(), 1) + ",";
  json += "\"ground_course\":" + String(bateau.get_ground_course(), 1) + ",";
  json += "\"water_depthMeters\":" + String(bateau.get_water_depthMeters(), 1) + ",";
  json += "\"water_speedKnots\":" + String(bateau.get_water_speedKnots(), 1) + ",";
  json += "\"water_temperatureCelsius\":" + String(bateau.get_water_temperatureCelsius(), 1) + ",";
  json += "\"wind_angle\":" + String(bateau.get_wind_angle(), 1) + ",";
  json += "\"wind_angleReference\":\"" + String(bateau.get_wind_angleReference()) + "\",";
  json += "\"wind_speedKts\":" + String(bateau.get_wind_speedKts(), 1);
  json += "}";
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: application/json");
  client.println("Connection: close");
  client.println();
  client.print(json);
}

void sendNotFound(WiFiClient &client) {
  client.println("HTTP/1.1 404 Not Found");
  client.println("Content-type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println("404 Not Found");
}