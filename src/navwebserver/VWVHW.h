#ifndef VWVHW_H
#define VWVHW_H

#include <Arduino.h>

struct VWVHW_Data {
  float   water_speedKnots;
  char    water_speedKnotsUnit;
  float   water_speedKmh;
  char    water_speedKmhUnit;
};

VWVHW_Data parseVWVHW(String nmea);

#endif
