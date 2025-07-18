#include "WIMTW.h"



WIMTW_Data parseWIMTW(String nmea) {
  WIMTW_Data data;

  if (!nmea.startsWith("$WIMTW")) {
    Serial.println("Erreur: pas une phrase WIMTW");
    return data;
  }

  int asteriskIndex = nmea.indexOf('*');
  if (asteriskIndex != -1) {
    nmea = nmea.substring(0, asteriskIndex);
  }

  String fields[5];
  int count = 0;
  int start = 0;
  int commaIndex;

  while ((commaIndex = nmea.indexOf(',', start)) != -1 && count < 5) {
    fields[count++] = nmea.substring(start, commaIndex);
    start = commaIndex + 1;
  }
  fields[count++] = nmea.substring(start);

  if (count < 3) return data;

  data.water_temperatureCelsius = fields[1].toFloat();
  data.water_temperatureUnit = fields[2].length() > 0 ? fields[2].charAt(0) : ' ';

  return data;
}


// //old version
// WIMTW_Data parseWIMTW(String nmea) {
//   WIMTW_Data data;
//   if (!nmea.startsWith("$WIMTW")) return data;
//   nmea.trim();

//   int lastIndex = 0;
//   int index = 0;
//   int fieldIndex = 0;

//   while ((index = nmea.indexOf(',', lastIndex)) != -1) {
//     String field = nmea.substring(lastIndex, index);
//     lastIndex = index + 1;

//     switch (fieldIndex) {
//       case 1: data.water_temperatureCelsius = field.toFloat(); break;
//       case 2: data.water_temperatureUnit = field.length() > 0 ? field.charAt(0) : ' '; break;
//     }

//     fieldIndex++;
//     if (fieldIndex > 2) break;
//   }

//   return data;
// }
