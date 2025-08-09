#ifndef WIMWV_H
#define WIMWV_H

#include <Arduino.h>
#include "config.h"


struct WIMWV_Data {
  bool    data_valid;
  float   wind_angle;
  char    wind_angleReference;     // R/T
  float   wind_speedKts;
  char    wind_speedUnit; // N/M/K
  char    wind_sensorStatus;        // A/V
};

WIMWV_Data parseWIMWV(String nmea);

#endif
