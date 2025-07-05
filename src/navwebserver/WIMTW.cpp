#include "WIMTW.h"

WIMTW_Data parseWIMTW(String nmea) {
  WIMTW_Data data;
  if (!nmea.startsWith("$WIMTW")) return data;
  nmea.trim();

  int lastIndex = 0;
  int index = 0;
  int fieldIndex = 0;

  while ((index = nmea.indexOf(',', lastIndex)) != -1) {
    String field = nmea.substring(lastIndex, index);
    lastIndex = index + 1;

    switch (fieldIndex) {
      case 1: data.water_temperatureCelsius = field.toFloat(); break;
      case 2: data.water_temperatureUnit = field.length() > 0 ? field.charAt(0) : ' '; break;
    }

    fieldIndex++;
    if (fieldIndex > 2) break;
  }

  return data;
}
