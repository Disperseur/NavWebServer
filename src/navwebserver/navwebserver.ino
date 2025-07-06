#include "NMEA.h"
#include "NMEAServer.h"

Nmea bateau;
NMEAServer server;

void setup() {
  Serial.begin(460800);
  Serial1.begin(460800);
  server.init("Saint-Lou_Wifi", "123456789");
}

void loop() {
  // Parse NMEA depuis Serial1
  while (Serial1.available()) {
    String msg = Serial1.readStringUntil('\n');
    bateau.parse(msg);
  }

  server.handleClient(bateau);
}
