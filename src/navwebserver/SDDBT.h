#ifndef SDDBT_H
#define SDDBT_H

#include <Arduino.h>
#include "config.h"


struct SDDBT_Data {
  bool  data_valid;
  float water_depthFeet;
  float water_depthMeters;
  float water_depthFathoms;
};

SDDBT_Data parseSDDBT(String nmea);

#endif
