#include "VWVHW.h"

VWVHW_Data parseVWVHW(String nmea) {
  VWVHW_Data data;

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

  data.water_speedKnots =     fields[5].toFloat();
  data.water_speedKnotsUnit = fields[6].length() > 0 ? fields[7].charAt(0) : ' ';
  data.water_speedKmh =       fields[7].toFloat();
  data.water_speedKmhUnit =   fields[8].length() > 0 ? fields[9].charAt(0) : ' ';

  return data;
}


// //old version
// VWVHW_Data parseVWVHW(String nmea) {
//   VWVHW_Data data;
//   if (!nmea.startsWith("$VWVHW")) return data;
//   nmea.trim();

//   int lastIndex = 0;
//   int index = 0;
//   int fieldIndex = 0;

//   while ((index = nmea.indexOf(',', lastIndex)) != -1) {
//     String field = nmea.substring(lastIndex, index);
//     lastIndex = index + 1;

//     switch (fieldIndex) {
//       case 5: data.water_speedKnots = field.toFloat(); break;
//       case 6: data.water_speedKnotsUnit = field.length() > 0 ? field.charAt(0) : ' '; break;
//       case 7: data.water_speedKmh = field.toFloat(); break;
//       case 8: data.water_speedKmhUnit = field.length() > 0 ? field.charAt(0) : ' '; break;
//     }

//     fieldIndex++;
//     if (fieldIndex > 8) break;
//   }

//   return data;
// }
