#include "NMEA.h"
#include "NMEAServer.h"
#include "config.h"
#include <USBHostSerialDevice.h>


//capteur pression
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>




using namespace rtos;


void pressure_alarm_reset(void);
Adafruit_BME280 bme; // I2C



Thread ledThread(osPriorityLow);
Thread serverThread(osPriorityNormal);
Thread pressureAlarmThread(osPriorityLow);

Nmea bateau;
NMEAServer server;
mbed::AnalogIn mcuADCTemp(ADC_TEMP); // pour la mesure de la temperature MCU
USBHostSerialDevice hser(true);




void ledThreadEntryPoint();
void serverThreadEntryPoint();
void pressureAlarmThreadEntryPoint();

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


  int status = bme.begin(0x76, &Wire1);
  if (!status) {
      while (1) {
          Serial.println("Erreur d'initialisation du bme");
          delay(10);
      }
  }
}




void loop() {
  bateau.parse(get_nmea_from_usbhost(hser));

  int mcuTemp = __HAL_ADC_CALC_TEMPERATURE (3300, mcuADCTemp.read_u16(), ADC_RESOLUTION_16B);

  Serial.print("MCU Temp : ");
  Serial.print(mcuTemp);
  Serial.println(" *C");

  delay(100);
  //ThisThread::sleep_for(100);
}




void pressureAlarmThreadEntryPoint() {

  pinMode(5, OUTPUT); //buzzer pin
  digitalWrite(5, LOW);
  pinMode(PC_13, INPUT);
  attachInterrupt(digitalPinToInterrupt(PC_13), pressure_alarm_reset, RISING);
  
  float oldTemp = bme.readTemperature();

  while(true) {
    if(bme.readTemperature() - oldTemp > 2.0) {
      digitalWrite(5, HIGH);
    }
    oldTemp = bme.readTemperature();

    ThisThread::sleep_for(4000); // delai a modifier pour travailler sur 30 minutes
  }
}


void pressure_alarm_reset(void) {
    digitalWrite(5, LOW);
}



void ledThreadEntryPoint() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  while(1) {
    digitalWrite(LED_GREEN, LOW);
    ThisThread::sleep_for(500);
    digitalWrite(LED_GREEN, HIGH);
    ThisThread::sleep_for(500);
  }
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