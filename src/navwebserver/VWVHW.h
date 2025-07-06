#ifndef VWVHW_H
#define VWVHW_H

#include <Arduino.h>

struct VWVHW_Data {
  int   water_speedKnots;
  char    water_speedKnotsUnit;
  int   water_speedKmh;
  char    water_speedKmhUnit;
};

VWVHW_Data parseVWVHW(String nmea);

#endif
