#include "NMEA.h"



Nmea bateau;

void setup() {
  Serial.begin(460800); // debug com
  Serial1.begin(460800); // data serial port on RX0
}

void loop() {
  while(Serial1.available()) {
    String nmea = Serial1.readStringUntil('\n');

#ifdef DEBUG
    Serial.println(nmea);
#endif

    bateau.parse(nmea);
  }

  Serial.println("");
  // Serial.println("Statut anemometre: " + String(bateau.get_wind_sensorStatus()));
  Serial.println("Statut GPS: " + String(bateau.get_ground_sensorStatus()));

  Serial.println("");
  Serial.println("Date: " + String(bateau.get_ground_date()));
  Serial.println("Heure UTC: " + bateau.get_ground_time());
  Serial.println("Temps de navigation: " + String(bateau.get_running_time()) + " s");

  Serial.println("");
  Serial.println("Latitude: " + bateau.get_ground_latitude() + " " + String(bateau.get_ground_latDir()));
  Serial.println("Longitude: " + bateau.get_ground_longitude() + " " + String(bateau.get_ground_longDir()));
  Serial.println("Vitesse sol: " + String(bateau.get_ground_speedKts()) + " kt");
  Serial.println("Cap: " + String(bateau.get_ground_course()) + "°");
  
  Serial.println("");
  Serial.println("Profondeur sous quille : " + String(bateau.get_water_depthMeters()) + " m");
  Serial.println("Vitesse dans l'eau: " + String(bateau.get_water_speedKnots()) + " kt");
  Serial.println("Temperature de l'eau: " + String(bateau.get_water_temperatureCelsius()) + " °C");

  Serial.println("");
  Serial.println("Direction du vent: " + String(bateau.get_wind_angle()) + "°");
  Serial.println("Vitesse du vent: " + String(bateau.get_wind_speedKts()) + " kt");


  delay(500);
}
