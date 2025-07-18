#include "SDDBT.h"




SDDBT_Data parseSDDBT(String nmea) {
  SDDBT_Data data;

  if (!nmea.startsWith("$SDDBT")) {
    Serial.println("Erreur: pas une phrase SDDBT");
    return data;
  }

  // Supprimer checksum
  int asteriskIndex = nmea.indexOf('*');
  if (asteriskIndex != -1) {
    nmea = nmea.substring(0, asteriskIndex);
  }

  // Split
  String fields[10];
  int count = 0;
  int start = 0;
  int commaIndex;

  while ((commaIndex = nmea.indexOf(',', start)) != -1 && count < 10) {
    fields[count++] = nmea.substring(start, commaIndex);
    start = commaIndex + 1;
  }
  fields[count++] = nmea.substring(start); // dernier champ

  if (count < 6) return data;

  data.water_depthFeet =    round(fields[1].toFloat());
  data.water_depthMeters =  round(fields[3].toFloat());
  data.water_depthFathoms = round(fields[5].toFloat());

  return data;
}


// //old version
// SDDBT_Data parseSDDBT(String nmea) {
//   SDDBT_Data data;
//   if (!nmea.startsWith("$SDDBT")) return data;
//   nmea.trim();

//   int lastIndex = 0;
//   int index = 0;
//   int fieldIndex = 0;

//   while ((index = nmea.indexOf(',', lastIndex)) != -1) {
//     String field = nmea.substring(lastIndex, index);
//     lastIndex = index + 1;

//     switch (fieldIndex) {
//       case 1: data.water_depthFeet = round(field.toFloat()); break;
//       case 3: data.water_depthMeters = round(field.toFloat()); break;
//       case 5: data.water_depthFathoms = round(field.toFloat()); break;
//     }

//     fieldIndex++;
//   }

//   return data;
// }
