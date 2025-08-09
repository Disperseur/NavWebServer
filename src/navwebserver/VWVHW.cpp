#include "VWVHW.h"

VWVHW_Data parseVWVHW(String nmea) {
  VWVHW_Data data;

  data.data_valid = false;

  if (!nmea.startsWith("$VWVHW")) {
    Serial.print(nmea);
    Serial.println(" : Erreur: pas une phrase VWVHW");
    return data;
  }

  int asteriskIndex = nmea.indexOf('*');
  if (asteriskIndex != -1) {
    nmea = nmea.substring(0, asteriskIndex);
  }

  String fields[15];
  int count = 0;
  int start = 0;
  int commaIndex;

  while ((commaIndex = nmea.indexOf(',', start)) != -1 && count < 15) {
    fields[count++] = nmea.substring(start, commaIndex);
    start = commaIndex + 1;
  }
  fields[count++] = nmea.substring(start);

  if (count < 7) return data;

  data.water_speedKts     = fields[5].toFloat();
  data.water_speedKtsUnit = fields[6].length() > 0 ? fields[7].charAt(0) : ' ';
  data.water_speedKmh     = fields[7].toFloat();
  data.water_speedKmhUnit = fields[8].length() > 0 ? fields[9].charAt(0) : ' ';
  data.data_valid         = true;

  return data;
}