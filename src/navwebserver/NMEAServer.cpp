#include "NMEAServer.h"

NMEAServer::NMEAServer()
: server(80) // initialiser le serveur sur le port 80
{
}

void NMEAServer::init(const char* ssid, const char* password) {
  Serial.println("Démarrage point d'accès : " + String(ssid));
  
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Erreur : pas de module WiFi");
    while (true);
  }

  int status = WiFi.beginAP(ssid, password);
  if (status != WL_AP_LISTENING) {
    Serial.println("Erreur : impossible de créer l'AP");
    while (true);
  }

  Serial.println("SSID: " + String(WiFi.SSID()));
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void NMEAServer::handleClient(Nmea &bateau) {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connecté");

    String request = client.readStringUntil('\r');
    client.read(); // consommer le '\n'
    Serial.println("Request: " + request);

    if (request.indexOf("GET / ") >= 0) {
      sendHTML(client);
    } else if (request.indexOf("GET /data") >= 0) {
      sendJSON(bateau, client);
    } else {
      sendNotFound(client);
    }

    delay(1);
    client.stop();
    Serial.println("Client déconnecté");
  }
}

void NMEAServer::sendHTML(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.print(INDEX_HTML);
}

void NMEAServer::sendJSON(Nmea &bateau, WiFiClient &client) {
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

void NMEAServer::sendNotFound(WiFiClient &client) {
  client.println("HTTP/1.1 404 Not Found");
  client.println("Content-type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println("404 Not Found");
}
