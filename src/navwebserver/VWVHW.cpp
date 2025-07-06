#include "VWVHW.h"

VWVHW_Data parseVWVHW(String nmea) {
  VWVHW_Data data;
  if (!nmea.startsWith("$VWVHW")) return data;
  nmea.trim();

  int lastIndex = 0;
  int index = 0;
  int fieldIndex = 0;

  while ((index = nmea.indexOf(',', lastIndex)) != -1) {
    String field = nmea.substring(lastIndex, index);
    lastIndex = index + 1;

    switch (fieldIndex) {
      case 5: data.water_speedKnots = round(field.toFloat()); break;
      case 6: data.water_speedKnotsUnit = field.length() > 0 ? field.charAt(0) : ' '; break;
      case 7: data.water_speedKmh = round(field.toFloat()); break;
      case 8: data.water_speedKmhUnit = field.length() > 0 ? field.charAt(0) : ' '; break;
    }

    fieldIndex++;
    if (fieldIndex > 8) break;
  }

  return data;
}
