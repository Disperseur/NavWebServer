#ifndef SDDBT_H
#define SDDBT_H

#include <Arduino.h>

struct SDDBT_Data {
  int water_depthFeet;
  int water_depthMeters;
  int water_depthFathoms;
};

SDDBT_Data parseSDDBT(String nmea);

#endif
