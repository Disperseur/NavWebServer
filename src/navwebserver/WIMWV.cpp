#include "WIMWV.h"


WIMWV_Data parseWIMWV(String nmea) {
  WIMWV_Data data;

  if (!nmea.startsWith("$WIMWV")) {
    Serial.println("Erreur: pas une phrase WIMWV");
    return data;
  }

  int asteriskIndex = nmea.indexOf('*');
  if (asteriskIndex != -1) {
    nmea = nmea.substring(0, asteriskIndex);
  }

  String fields[10];
  int count = 0;
  int start = 0;
  int commaIndex;

  while ((commaIndex = nmea.indexOf(',', start)) != -1 && count < 10) {
    fields[count++] = nmea.substring(start, commaIndex);
    start = commaIndex + 1;
  }
  fields[count++] = nmea.substring(start);

  if (count < 6) return data;

  data.wind_angle =           round(fields[1].toFloat());
  data.wind_angleReference =  fields[2].length() > 0 ? fields[2].charAt(0) : ' ';
  data.wind_speedKts =        round(fields[3].toFloat());
  data.wind_speedUnit =       fields[4].length() > 0 ? fields[4].charAt(0) : ' ';
  data.wind_sensorStatus =    fields[5].length() > 0 ? fields[5].charAt(0) : ' ';

  return data;
}


// //old version
// WIMWV_Data parseWIMWV(String nmea) {
//   WIMWV_Data data;
//   if (!nmea.startsWith("$WIMWV")) return data;
//   nmea.trim();

//   int lastIndex = 0;
//   int index = 0;
//   int fieldIndex = 0;

//   while ((index = nmea.indexOf(',', lastIndex)) != -1) {
//     String field = nmea.substring(lastIndex, index);
//     lastIndex = index + 1;

//     switch (fieldIndex) {
//       case 1: data.wind_angle = round(field.toFloat()); break;
//       case 2: data.wind_angleReference = field.length() > 0 ? field.charAt(0) : ' '; break;
//       case 3: data.wind_speedKts = round(field.toFloat()); break;
//       case 4: data.wind_speedUnit = field.length() > 0 ? field.charAt(0) : ' '; break;
//       case 5: data.wind_sensorStatus = field.length() > 0 ? field.charAt(0) : ' '; break;
//     }

//     fieldIndex++;
//     if (fieldIndex > 5) break;
//   }

//   return data;
// }
