#include "NMEA.h"
#include "NMEAServer.h"
#include "config.h"
#include "Alarm.h"
#include "LedService.h"

#include <USBHostSerialDevice.h>


using namespace rtos;


Thread ledThread(osPriorityLow);
Thread serverThread(osPriorityNormal);
Thread pressureAlarmThread(osPriorityLow);

Nmea bateau;
NMEAServer server;
mbed::AnalogIn mcuADCTemp(ADC_TEMP); // pour la mesure de la temperature MCU
USBHostSerialDevice hser(true);


void serverThreadEntryPoint();

String get_nmea_from_usbhost(USBHostSerialDevice &dev);



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

  server.init("Saint-Lou_Wifi", "123456789"); //SSID, PASSWORD

  ledThread.start(ledThreadEntryPoint); // status led start
  serverThread.start(serverThreadEntryPoint);
  pressureAlarmThread.start(pressureAlarmThreadEntryPoint);

}




void loop() {
  bateau.parse(get_nmea_from_usbhost(hser));

  int mcuTemp = __HAL_ADC_CALC_TEMPERATURE (3300, mcuADCTemp.read_u16(), ADC_RESOLUTION_16B);

  Serial.print("MCU Temp : ");
  Serial.print(mcuTemp);
  Serial.println(" *C");

  ThisThread::sleep_for(100);
}














void serverThreadEntryPoint() {
  while (true) {
    server.handleClient(bateau);
    ThisThread::sleep_for(10); // (ms) pour éviter de saturer le CPU
  }
}



String get_nmea_from_usbhost(USBHostSerialDevice &dev) {
  String incomingLine = "";  // Buffer pour construire une ligne complète

  while (dev.available()) {
    char c = dev.read();

    if (c == '\n') {  // Fin de trame NMEA
      incomingLine.trim(); // enlève '\r' ou espaces

      if (incomingLine.length() > 0) {
        // On a une trame complète, on la revoie
        return incomingLine;
      }

      incomingLine = ""; // Reset pour la prochaine trame
    } else {
      incomingLine += c; // Ajoute le caractère au buffer
    }
  }
}



