#include "Alarm.h"


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