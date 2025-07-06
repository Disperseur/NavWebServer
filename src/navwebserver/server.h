#ifndef SERVER_H
#define SERVER_H

#include "Arduino.h"
#include <SPI.h>
#include <WiFi.h>

#include "NMEA.h"
#include "page1.h"

void sendHTML(WiFiClient &client);
void sendJSON(Nmea &bateau, WiFiClient &client);
void sendNotFound(WiFiClient &client);

#endif