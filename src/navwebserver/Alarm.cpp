#include "Alarm.h"


void buzzerAlarmWatcherEntryPoint(void* arg) {
    Nmea* bateau = (Nmea*)arg;

    pinMode(BUZZER_PIN, OUTPUT); //buzzer pin
    digitalWrite(BUZZER_PIN, LOW);

    while(true) {
        if(bateau->get_depth_alarm() || bateau->get_pressure_alarm()) {
            digitalWrite(BUZZER_PIN, HIGH);
            ThisThread::sleep_for(200);
            digitalWrite(BUZZER_PIN, LOW);
            ThisThread::sleep_for(200);
        } else {
            ThisThread::sleep_for(1000);
        }
    }
}

void depthAlarmThreadEntryPoint(void* arg) {
    Nmea* bateau = (Nmea*)arg;

    float old_depth = bateau->get_water_depthMeters();

    while(true) {
        if( (bateau->get_water_depthMeters() < 2.0) && (old_depth > 2.0) ) {
            bateau->set_depth_alarm(true);
        }

        old_depth = bateau->get_water_depthMeters();

        ThisThread::sleep_for(2000);
    }
}

void pressureAlarmThreadEntryPoint(void* arg) {
  Nmea* bateau = (Nmea*)arg;

  Adafruit_BME280 bme; // I2C

  int status = bme.begin(0x76, &Wire1);
  if (!status) {
      while (1) {
          Serial1.println("Erreur d'initialisation du bme");
          delay(10);
      }
  }
  
  float oldTemp = bme.readTemperature();
  bateau->set_pressure_alarm(false);

  Serial1.println("[ALARM] Service started.");

  while(true) {
#ifdef DEBUG_ALARM
    Serial1.print("[ALARME] Temperature mesuree : ");
    Serial1.print(bme.readTemperature());
    Serial1.println(" *C");
#endif

    if(bme.readTemperature() > 33.0) { // (bme.readTemperature() - oldTemp > 2.0)
      bateau->set_pressure_alarm(true);
    }
    oldTemp = bme.readTemperature();

    ThisThread::sleep_for(4000); // delai a modifier pour travailler sur 30 minutes
  }
}