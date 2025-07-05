#include "GPRMC.h"

// Convertit un champ NMEA brut en String formatée : DD°MM.mm' ou DDD°MM.mm'
String formatNMEACoordinate(String raw, Coord_t coord_type) {
  if (raw.length() < 4) return "";

  int degreeLen; // longitude = 3 chiffres, latitude = 2
  if(coord_type == LAT) degreeLen = 2;
  else if(coord_type == LON) degreeLen = 3;


  String degrees = raw.substring(0, degreeLen);
  String minutes = raw.substring(degreeLen);

  String result = degrees + "°" + minutes + "'";
  return result;
}


// Convertit hhmmss.sss en HH:MM:SS
String formatNMEATime(String rawTime) {
  if (rawTime.length() < 6) return "";
  String hour = rawTime.substring(0, 2);
  String minute = rawTime.substring(2, 4);
  String second = rawTime.substring(4, 6);
  return hour + ":" + minute + ":" + second;
}

// Convertit ddmmyy en JJ/MM/AAAA
String formatNMEADate(String rawDate) {
  if (rawDate.length() != 6) return "";
  String day = rawDate.substring(0, 2);
  String month = rawDate.substring(2, 4);
  String year = rawDate.substring(4, 6);

  // Convertir année sur 2 chiffres en année sur 4 chiffres
  // Si tu veux gérer 1900 vs 2000, tu peux affiner.
  int y = year.toInt();
  String fullYear = (y < 70 ? "20" : "19") + year; // par ex: "24" → "2024"

  return day + "/" + month + "/" + fullYear;
}








GPRMC_Data parseGPRMC(String nmea) {
  GPRMC_Data data;
  if (!nmea.startsWith("$GPRMC")) return data;
  nmea.trim();

  int lastIndex = 0;
  int index = 0;
  int fieldIndex = 0;
  String rawLat, rawLon, rawTime, rawDate;

  while ((index = nmea.indexOf(',', lastIndex)) != -1) {
    String field = nmea.substring(lastIndex, index);
    lastIndex = index + 1;

    switch (fieldIndex) {
      case 1: rawTime = field; break;
      case 2: data.ground_sensorStatus = field.length() > 0 ? field.charAt(0) : 'V'; break;
      case 3: rawLat = field; break;
      case 4: data.ground_latDir = field.length() > 0 ? field.charAt(0) : ' '; break;
      case 5: rawLon = field; break;
      case 6: data.ground_lonDir = field.length() > 0 ? field.charAt(0) : ' '; break;
      case 7: data.ground_speedKts = field.toFloat(); break;
      case 8: data.ground_course = field.toFloat(); break;
      case 9: rawDate = field; break;
    }

    fieldIndex++;
    if (fieldIndex > 9) break;
  }

  // formatage latitude et longitude
  data.ground_time = formatNMEATime(rawTime);
  data.ground_date = formatNMEADate(rawDate);
  data.ground_latitude = formatNMEACoordinate(rawLat, LAT);
  data.ground_longitude = formatNMEACoordinate(rawLon, LON);
  return data;
}
