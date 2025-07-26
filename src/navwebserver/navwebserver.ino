#include "NMEA.h"
#include "NMEAServer.h"
#include "config.h"
#include "Alarm.h"
#include "LedService.h"



using namespace rtos;


Thread ledThread(osPriorityLow);
Thread serverThread(osPriorityNormal);
Thread pressureAlarmThread(osPriorityLow);

Nmea bateau;
NMEAServer server;
mbed::AnalogIn mcuADCTemp(ADC_TEMP); // pour la mesure de la temperature MCU
USBHostSerialDevice hser(true);


void serverThreadEntryPoint();
void pressure_alarm_reset(void);
void pressureAlarmThreadEntryPoint();




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

  // Serial.print("MCU Temp : ");
  // Serial.print(mcuTemp);
  // Serial.println(" *C");

  ThisThread::sleep_for(100);
}




void serverThreadEntryPoint() {
  while (true) {
    server.handleClient(bateau);
    ThisThread::sleep_for(10); // (ms) pour éviter de saturer le CPU
  }
}





void pressureAlarmThreadEntryPoint() {
  Adafruit_BME280 bme; // I2C

  pinMode(5, OUTPUT); //buzzer pin
  digitalWrite(5, LOW);
  pinMode(PC_13, INPUT);
  attachInterrupt(digitalPinToInterrupt(PC_13), pressure_alarm_reset, RISING);


  int status = bme.begin(0x76, &Wire1);
  if (!status) {
      while (1) {
          Serial.println("Erreur d'initialisation du bme");
          delay(10);
      }
  }
  
  float oldTemp = bme.readTemperature();
  bateau.set_pressure_alarm(false);

  while(true) {
#ifdef DEBUG_ALARM
    Serial.print(bme.readTemperature());
#endif

    if(bme.readTemperature() > 33.0) { // (bme.readTemperature() - oldTemp > 2.0)
      digitalWrite(5, HIGH);
      bateau.set_pressure_alarm(true);
    }
    oldTemp = bme.readTemperature();

    ThisThread::sleep_for(4000); // delai a modifier pour travailler sur 30 minutes
  }
}


void pressure_alarm_reset(void) {
    digitalWrite(5, LOW);
    bateau.set_pressure_alarm(false);
}