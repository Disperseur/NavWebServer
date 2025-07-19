#include "NMEA.h"

// fonctions utilisateur


String millisToTimeString(unsigned long ms) {
  unsigned long totalSeconds = ms / 1000;
  unsigned int hours = totalSeconds / 3600;
  unsigned int minutes = (totalSeconds % 3600) / 60;
  unsigned int seconds = totalSeconds % 60;

  // Formater avec des zéros devant si nécessaire
  char buffer[9]; // "HH:MM:SS"
  sprintf(buffer, "%02u:%02u:%02u", hours, minutes, seconds);
  return String(buffer);
}

int Nmea::parse(String nmea) {
  running_time = millisToTimeString(millis());


  if(nmea.indexOf("GPRMC") != -1) {
    getGPRMCData(nmea);
  }
  else if(nmea.indexOf("SDDBT") != -1) {
    getSDDBTData(nmea);
  }
  else if(nmea.indexOf("VWVHW") != -1) {
    getVWVHWData(nmea);
  }
  else if(nmea.indexOf("WIMTW") != -1) {
    getWIMTWData(nmea);
  }
  else if(nmea.indexOf("WIMWV") != -1) {
    getWIMWVData(nmea);
  }
}






void Nmea::getGPRMCData(String nmea) {
  GPRMC_Data result = parseGPRMC(nmea);
  set_ground_time(result.ground_time);
  set_ground_sensorStatus(result.ground_sensorStatus);
  set_ground_latitude(result.ground_latitude);
  set_ground_latDir(result.ground_latDir);
  set_ground_longitude(result.ground_longitude);
  set_ground_longDir(result.ground_lonDir);
  set_ground_speedKts(result.ground_speedKts);
  set_ground_course(result.ground_course);
  set_ground_date(result.ground_date);
}

void Nmea::getSDDBTData(String nmea) {
  SDDBT_Data result = parseSDDBT(nmea);
  set_water_depthMeters(result.water_depthMeters);
}

void Nmea::getVWVHWData(String nmea) {
  VWVHW_Data result = parseVWVHW(nmea);
  set_water_speedKnots(result.water_speedKnots);
}

void Nmea::getWIMTWData(String nmea) {
  WIMTW_Data result = parseWIMTW(nmea);
  set_water_temperatureCelsius(result.water_temperatureCelsius);
}

void Nmea::getWIMWVData(String nmea) {
  WIMWV_Data result = parseWIMWV(nmea);
  set_wind_angle(result.wind_angle);
  set_wind_angleReference(result.wind_angleReference);
  set_wind_speedKts(result.wind_speedKts);
  set_wind_sensorStatus(result.wind_sensorStatus);
}



void Nmea::printData(void) {
  Serial.println("");
  // Serial.println("Statut anemometre: " + String(bateau.get_wind_sensorStatus()));
  Serial.println("Statut GPS: " + String(get_ground_sensorStatus()));

  Serial.println("");
  Serial.println("Date: " + String(get_ground_date()));
  Serial.println("Heure UTC: " + get_ground_time());
  Serial.println("Temps de navigation: " + String(get_running_time()) + " s");

  Serial.println("");
  Serial.println("Latitude: " + get_ground_latitude() + " " + String(get_ground_latDir()));
  Serial.println("Longitude: " + get_ground_longitude() + " " + String(get_ground_longDir()));
  Serial.println("Vitesse sol: " + String(get_ground_speedKts()) + " kt");
  Serial.println("Cap: " + String(get_ground_course()) + "°");
  
  Serial.println("");
  Serial.println("Profondeur sous quille : " + String(get_water_depthMeters()) + " m");
  Serial.println("Vitesse dans l'eau: " + String(get_water_speedKnots()) + " kt");
  Serial.println("Temperature de l'eau: " + String(get_water_temperatureCelsius()) + " °C");

  Serial.println("");
  Serial.println("Direction du vent: " + String(get_wind_angle()) + "°");
  Serial.println("Vitesse du vent: " + String(get_wind_speedKts()) + " kt");
}

// Mutateurs et accesseurs

int Nmea::set_ground_time(String t) {
  ground_time = t;
}

int Nmea::set_ground_date(String d) {
  ground_date = d;
}

int Nmea::set_wind_sensorStatus(char s) {
  wind_sensorStatus = s;
}

int Nmea::set_ground_sensorStatus(char s) {
  ground_sensorStatus = s;
}

int Nmea::set_ground_latitude(String lat) {
  ground_latitude = lat;
}

int Nmea::set_ground_latDir(char latD) {
  ground_latDir = latD;
}

int Nmea::set_ground_longitude(String lon) {
  ground_longitude = lon;
}

int Nmea::set_ground_longDir(char lonD) {
  ground_longDir = lonD;
}

int Nmea::set_ground_speedKts(float s) {
  ground_speedKts = s;
}

int Nmea::set_ground_course(float c) {
  ground_course = c;
}

int Nmea::set_water_depthMeters(float d) {
  water_depthMeters = d;
}

int Nmea::set_water_speedKnots(float s) {
  water_speedKnots = s;
}

int Nmea::set_water_temperatureCelsius(float t) {
  water_temperatureCelsius = t;
}

int Nmea::set_wind_angle(float a) {
  wind_angle = a;
}

int Nmea::set_wind_angleReference(char ar) {
  wind_angleReference = ar;
}

int Nmea::set_wind_speedKts(float s) {
  wind_speedKts = s;
}




String Nmea::get_running_time(void) {
  return running_time;
}

String Nmea::get_ground_time(void) {
  return ground_time;
}

String Nmea::get_ground_date(void) {
  return ground_date;
}

char Nmea::get_wind_sensorStatus(void) {
  return wind_sensorStatus;
}

char Nmea::get_ground_sensorStatus(void) {
  return ground_sensorStatus;
}

String Nmea::get_ground_latitude(void) {
  return ground_latitude;
}

char Nmea::get_ground_latDir(void) {
  return ground_latDir;
}

String Nmea::get_ground_longitude(void) {
  return ground_longitude;
}

char Nmea::get_ground_longDir(void) {
  return ground_longDir;
}

float Nmea::get_ground_speedKts(void) {
  return ground_speedKts;
}

float Nmea::get_ground_course(void) {
  return ground_course;
}

float Nmea::get_water_depthMeters(void) {
  return water_depthMeters;
}

float Nmea::get_water_speedKnots(void) {
  return water_speedKnots;
}

float Nmea::get_water_temperatureCelsius(void) {
  return water_temperatureCelsius;
}

float Nmea::get_wind_angle(void) {
  return wind_angle;
}

char Nmea::get_wind_angleReference(void) {
  return wind_angleReference;
}

float Nmea::get_wind_speedKts(void) {
  return wind_speedKts;
}