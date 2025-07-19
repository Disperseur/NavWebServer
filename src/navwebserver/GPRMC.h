#ifndef GPRMC_H
#define GPRMC_H

#include <Arduino.h>
#include "config.h"


typedef enum {LAT, LON} Coord_t;

struct GPRMC_Data {
  String  ground_time;
  char    ground_sensorStatus;
  String  ground_latitude;
  char    ground_latDir;
  String  ground_longitude;
  char    ground_lonDir;
  float   ground_speedKts;  // en noeuds
  float   ground_course; // en degrés
  String  ground_date;
};



GPRMC_Data parseGPRMC(String nmea);
String formatNMEACoordinate(String raw, Coord_t coord_type);
String formatNMEATime(String rawTime);
String formatNMEADate(String rawDate);

#endif
