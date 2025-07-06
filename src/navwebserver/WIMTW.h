#ifndef WIMTW_H
#define WIMTW_H

#include <Arduino.h>

struct WIMTW_Data {
  int   water_temperatureCelsius;
  char    water_temperatureUnit; // C
};

WIMTW_Data parseWIMTW(String nmea);

#endif
