#ifndef GPRMC_H
#define GPRMC_H

#include <Arduino.h>

struct GPRMC_Data {
  String  ground_time;
  char    ground_sensorStatus;
  float   ground_latitude;
  char    ground_latDir;
  float   ground_longitude;
  char    ground_lonDir;
  float   ground_speedKts;  // en noeuds
  float   ground_course; // en degrés
  String  ground_date;
};

GPRMC_Data parseGPRMC(String nmea);

#endif
