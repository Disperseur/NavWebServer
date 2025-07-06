#include "WIMWV.h"

WIMWV_Data parseWIMWV(String nmea) {
  WIMWV_Data data;
  if (!nmea.startsWith("$WIMWV")) return data;
  nmea.trim();

  int lastIndex = 0;
  int index = 0;
  int fieldIndex = 0;

  while ((index = nmea.indexOf(',', lastIndex)) != -1) {
    String field = nmea.substring(lastIndex, index);
    lastIndex = index + 1;

    switch (fieldIndex) {
      case 1: data.wind_angle = round(field.toFloat()); break;
      case 2: data.wind_angleReference = field.length() > 0 ? field.charAt(0) : ' '; break;
      case 3: data.wind_speedKts = round(field.toFloat()); break;
      case 4: data.wind_speedUnit = field.length() > 0 ? field.charAt(0) : ' '; break;
      case 5: data.wind_sensorStatus = field.length() > 0 ? field.charAt(0) : ' '; break;
    }

    fieldIndex++;
    if (fieldIndex > 5) break;
  }

  return data;
}
