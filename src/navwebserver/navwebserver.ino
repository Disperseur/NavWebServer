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

    if(nmea.indexOf("GPRMC") != -1) {
      GPRMC_Data result = parseGPRMC(nmea);
      bateau.set_ground_time(result.ground_time);
      bateau.set_ground_sensorStatus(result.ground_sensorStatus);
      bateau.set_ground_latitude(result.ground_latitude);
      bateau.set_ground_latDir(result.ground_latDir);
      bateau.set_ground_longitude(result.ground_longitude);
      bateau.set_ground_longDir(result.ground_lonDir);
      bateau.set_ground_speedKts(result.ground_speedKts);
      bateau.set_ground_course(result.ground_course);
      bateau.set_ground_date(result.ground_date);
    }
    else if(nmea.indexOf("SDDBT") != -1) {
      SDDBT_Data result = parseSDDBT(nmea);
      bateau.set_water_depthMeters(result.water_depthMeters);
    }
    else if(nmea.indexOf("VWVHW") != -1) {
      VWVHW_Data result = parseVWVHW(nmea);
      bateau.set_water_speedKnots(result.water_speedKnots);
    }
    else if(nmea.indexOf("WIMTW") != -1) {
      WIMTW_Data result = parseWIMTW(nmea);
      bateau.set_water_temperatureCelsius(result.water_temperatureCelsius);
    }
    else if(nmea.indexOf("WIMWV") != -1) {
      WIMWV_Data result = parseWIMWV(nmea);
      bateau.set_wind_angle(result.wind_angle);
      bateau.set_wind_angleReference(result.wind_angleReference);
      bateau.set_wind_speedKts(result.wind_speedKts);
      bateau.set_wind_sensorStatus(result.wind_sensorStatus);
    }
  }

  Serial.println("");
  Serial.println("Statut anemometre: " + String(bateau.get_wind_sensorStatus()));
  Serial.println("Statut GPS: " + String(bateau.get_ground_sensorStatus()));

  Serial.println("");
  Serial.println("Heure UTC: " + bateau.get_ground_time());
  Serial.println("Date: " + String(bateau.get_ground_date()));

  Serial.println("");
  Serial.println("Latitude: " + String(bateau.get_ground_latitude()) + " " + String(bateau.get_ground_latDir()));
  Serial.println("Longitude: " + String(bateau.get_ground_longitude()) + " " + String(bateau.get_ground_longDir()));
  Serial.println("Vitesse sol: " + String(bateau.get_ground_speedKts()) + " Kts");
  Serial.println("Cap: " + String(bateau.get_ground_course()) + " °");
  
  Serial.println("");
  Serial.println("Profondeur sous quille : " + String(bateau.get_water_depthMeters()) + " m");
  Serial.println("Vitesse dans l'eau: " + String(bateau.get_water_speedKnots()) + " Kts");
  Serial.println("Temperature de l'eau: " + String(bateau.get_water_temperatureCelsius()) + " °C");

  Serial.println("");
  Serial.println("Direction du vent: " + String(bateau.get_wind_angle()) + " °");
  Serial.println("Vitesse du vent: " + String(bateau.get_wind_speedKts()) + " Kts");


  delay(500);
}
