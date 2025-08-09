#ifndef VWVHW_H
#define VWVHW_H

#include <Arduino.h>
#include "config.h"


struct VWVHW_Data {
  bool    data_valid;
  float   water_speedKts;
  char    water_speedKtsUnit;
  float   water_speedKmh;
  char    water_speedKmhUnit;
};

VWVHW_Data parseVWVHW(String nmea);

#endif
