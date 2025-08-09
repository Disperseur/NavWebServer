#include "GPRMC.h"

// Convertit un champ NMEA brut en String formatée : DD°MM.mm' ou DDD°MM.mm'
String formatNMEACoordinate(String raw, Coord_t coord_type) {
  if (raw.length() < 4) return "";

  int degreeLen; // longitude = 3 chiffres, latitude = 2
  if(coord_type == LAT) degreeLen = 2;
  else if(coord_type == LON) degreeLen = 3;


  String degrees = raw.substring(0, degreeLen);
  String minutes = raw.substring(degreeLen, degreeLen+4);

  String result = degrees + "°" + minutes + "'";
  return result;
}


// Convertit hhmmss.sss en HH:MM:SS
String formatNMEATime(String rawTime) {
  if (rawTime.length() < 6) return "";
  String hour = rawTime.substring(0, 2);
  String minute = rawTime.substring(2, 4);
  String second = rawTime.substring(4, 6);
  return hour + ":" + minute; // + ":" + second;
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
  // Serial.println("Parsing GPRMC phrase...");

  GPRMC_Data data;
  data.data_valid = false;

  if (!nmea.startsWith("$GPRMC")) {
    Serial.print(nmea);
    Serial.println(" : Erreur: pas une phrase GPRMC");
    return data;
  }

  nmea.trim();
  
  // Enlever le checksum pour ne pas polluer le dernier champ
  int asteriskIndex = nmea.indexOf('*');
  if (asteriskIndex != -1) {
    nmea = nmea.substring(0, asteriskIndex);
  }

  // Split en champs
  String fields[20]; // il y a rarement plus de 20 champs
  int fieldCount = 0;
  int start = 0;
  int commaIndex;

  while ((commaIndex = nmea.indexOf(',', start)) != -1) {
    fields[fieldCount++] = nmea.substring(start, commaIndex);
    start = commaIndex + 1;
  }
  // Ajouter le dernier champ après la dernière virgule
  fields[fieldCount++] = nmea.substring(start);

  // Vérification pour éviter les index out of bounds
  if (fieldCount < 10) {
    Serial.print("Trame trop courte ! : ");
    Serial.println(nmea);
    return data;
  }

  data.ground_time =          formatNMEATime(fields[1]);
  data.ground_sensorStatus =  fields[2].length() > 0 ? fields[2].charAt(0) : 'V';
  data.ground_latitude  =     formatNMEACoordinate(fields[3], LAT);
  data.ground_latDir    =     fields[4].length() > 0 ? fields[4].charAt(0) : ' ';
  data.ground_longitude =     formatNMEACoordinate(fields[5], LON);
  data.ground_lonDir    =     fields[6].length() > 0 ? fields[6].charAt(0) : ' ';
  data.ground_speedKts     =  fields[7].toFloat();
  data.ground_course    =     fields[8].toFloat();
  data.ground_date      =     formatNMEADate(fields[9]);
  data.data_valid       =     true;

  return data;
}