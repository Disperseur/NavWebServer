#ifndef NMEASERVER_H
#define NMEASERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include "NMEA.h"
#include "page1.h"
#include "config.h"


// #define DEBUG_SERVER

class NMEAServer {
  public:
    NMEAServer();
    void init(const char* ssid, const char* password);
    void handleClient(Nmea &bateau);

  private:
    WiFiServer server;
    void sendHTML(WiFiClient &client);
    void sendJSON(Nmea &bateau, WiFiClient &client);
    void sendNotFound(WiFiClient &client);
};

void serverThreadEntryPoint();


#endif
