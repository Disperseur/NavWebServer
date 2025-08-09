#ifndef WIMTW_H
#define WIMTW_H

#include <Arduino.h>
#include "config.h"


struct WIMTW_Data {
  bool    data_valid;
  float   water_temperatureCelsius;
  char    water_temperatureUnit; // C
};

WIMTW_Data parseWIMTW(String nmea);

#endif
