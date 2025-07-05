#include "GPRMC.h"

GPRMC_Data parseGPRMC(String nmea) {
  GPRMC_Data data;
  if (!nmea.startsWith("$GPRMC")) return data;
  nmea.trim();

  int lastIndex = 0;
  int index = 0;
  int fieldIndex = 0;

  while ((index = nmea.indexOf(',', lastIndex)) != -1) {
    String field = nmea.substring(lastIndex, index);
    lastIndex = index + 1;

    switch (fieldIndex) {
      case 1: data.ground_time = field; break;
      case 2: data.ground_sensorStatus = field.length() > 0 ? field.charAt(0) : 'V'; break;
      case 3: data.ground_latitude = field.toFloat(); break;
      case 4: data.ground_latDir = field.length() > 0 ? field.charAt(0) : ' '; break;
      case 5: data.ground_longitude = field.toFloat(); break;
      case 6: data.ground_lonDir = field.length() > 0 ? field.charAt(0) : ' '; break;
      case 7: data.ground_speedKts = field.toFloat(); break;
      case 8: data.ground_course = field.toFloat(); break;
      case 9: data.ground_date = field; break;
    }

    fieldIndex++;
    if (fieldIndex > 9) break;
  }
  return data;
}
