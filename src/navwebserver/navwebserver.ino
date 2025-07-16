#include "NMEA.h"
#include "NMEAServer.h"
#include <USBHostSerialDevice.h>

#define BUFFER_SIZE 500

USBHostSerialDevice hser(true);
char buffer[BUFFER_SIZE];
int i = 0;

Nmea bateau;
NMEAServer server;

void setup() {
  Serial.begin(460800);
  // Serial1.begin(460800);
  

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

void loop() {
  // Parse NMEA depuis USBHost
  while (hser.available()) {
    buffer[i % BUFFER_SIZE] = hser.read();
    i++;
  }

  String msg = String(buffer);
  Serial.print(msg);
  memset(buffer, 0, BUFFER_SIZE);
  i = 0;

  bateau.parse(msg);
  // bateau.printData();

  // while (Serial1.available()) {
  //   String msg = Serial1.readStringUntil('\n');
  //   bateau.parse(msg);
  //   bateau.printData();
  // }

  server.handleClient(bateau);
}
