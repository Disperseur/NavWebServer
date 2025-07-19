#include "NMEA.h"
#include "NMEAServer.h"
#include <USBHostSerialDevice.h>

// #define DEBUG_MAIN

USBHostSerialDevice hser(true);


Nmea bateau;
NMEAServer server;

void setup() {
  Serial.begin(460800);
  

  // Enable the USBHost
  pinMode(PA_15, OUTPUT);
  digitalWrite(PA_15, HIGH);

  while (!hser.connect()) {
    Serial.println("No USB host Serial device connected");
    delay(1000);
  }
  hser.begin(460800);

  server.init("Saint-Lou_Wifi", "123456789");
}

String incomingLine = "";  // Buffer pour construire une ligne complète

void loop() {
  while (hser.available()) {
    char c = hser.read();

    if (c == '\n') {  // Fin de trame NMEA
      incomingLine.trim(); // enlève '\r' ou espaces

      if (incomingLine.length() > 0) {
        // On a une trame complète, on la parse
        bateau.parse(incomingLine);
        // bateau.printData(); // si tu veux

#ifdef DEBUG_MAIN
        Serial.print("Trame recue : ");
        Serial.println(incomingLine);
        Serial.println();
        Serial.print("Ground speed avg 30min : ");
        Serial.print(bateau.get_ground_speedKts_avg());
        Serial.println(" kts");

        Serial.print("Water speed avg 30min : ");
        Serial.print(bateau.get_water_speedKnots_avg());
        Serial.println(" kts");
#endif        
      }

      incomingLine = ""; // Reset pour la prochaine trame
    } else {
      incomingLine += c; // Ajoute le caractère au buffer
    }
  }

  // Gérer le serveur web à chaque loop
  server.handleClient(bateau);

  delay(50);
}