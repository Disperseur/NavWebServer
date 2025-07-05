#ifndef SDDBT_H
#define SDDBT_H

#include <Arduino.h>

struct SDDBT_Data {
  float water_depthFeet;
  float water_depthMeters;
  float water_depthFathoms;
};

SDDBT_Data parseSDDBT(String nmea);

#endif
