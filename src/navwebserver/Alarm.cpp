#include "Alarm.h"


void pressureAlarmThreadEntryPoint(void* arg) {
  Nmea* bateau = (Nmea*)arg;

  Adafruit_BME280 bme; // I2C

  pinMode(5, OUTPUT); //buzzer pin
  digitalWrite(5, LOW);

  // pinMode(PC_13, INPUT); //alarm off button
  // attachInterrupt(digitalPinToInterrupt(PC_13), cb_pressure_alarm_reset, RISING);


  int status = bme.begin(0x76, &Wire1);
  if (!status) {
      while (1) {
          Serial.println("Erreur d'initialisation du bme");
          delay(10);
      }
  }
  
  float oldTemp = bme.readTemperature();
  bateau->set_pressure_alarm(false);

  while(true) {
#ifdef DEBUG_ALARM
    Serial.print("[ALARME] Temperature mesuree : ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
#endif

    if(bme.readTemperature() > 33.0) { // (bme.readTemperature() - oldTemp > 2.0)
      digitalWrite(5, HIGH);
      bateau->set_pressure_alarm(true);
    }
    oldTemp = bme.readTemperature();

    ThisThread::sleep_for(4000); // delai a modifier pour travailler sur 30 minutes
  }
}

// void cb_pressure_alarm_reset(void) {
//     digitalWrite(5, LOW);
//     bateau.set_pressure_alarm(false);
// }