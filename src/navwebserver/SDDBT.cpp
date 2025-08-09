#include "SDDBT.h"




SDDBT_Data parseSDDBT(String nmea) {
  SDDBT_Data data;

  data.data_valid = false; 

  if (!nmea.startsWith("$SDDBT")) {
    Serial.print(nmea);
    Serial.println(" : Erreur: pas une phrase SDDBT");
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

  data.water_depthFeet    = fields[1].toFloat();
  data.water_depthMeters  = fields[3].toFloat();
  data.water_depthFathoms = fields[5].toFloat();
  data.data_valid         = true;

  return data;
}