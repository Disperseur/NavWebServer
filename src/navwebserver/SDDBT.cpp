#include "SDDBT.h"

SDDBT_Data parseSDDBT(String nmea) {
  SDDBT_Data data;
  if (!nmea.startsWith("$SDDBT")) return data;
  nmea.trim();

  int lastIndex = 0;
  int index = 0;
  int fieldIndex = 0;

  while ((index = nmea.indexOf(',', lastIndex)) != -1) {
    String field = nmea.substring(lastIndex, index);
    lastIndex = index + 1;

    switch (fieldIndex) {
      case 1: data.water_depthFeet = round(field.toFloat()); break;
      case 3: data.water_depthMeters = round(field.toFloat()); break;
      case 5: data.water_depthFathoms = round(field.toFloat()); break;
    }

    fieldIndex++;
  }

  return data;
}
