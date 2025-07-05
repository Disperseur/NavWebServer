#include "NMEA.h"

// fonctions utilisateur

String float_to_coord(float c) {
  /*Converti une latitude ou longitude de float a String formatée correctement*/
  int degrees = c / 100;
  return String(c)
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

int Nmea::set_ground_latitude(float lat) {
  ground_latitude = lat;
}

int Nmea::set_ground_latDir(char latD) {
  ground_latDir = latD;
}

int Nmea::set_ground_longitude(float lon) {
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

float Nmea::get_ground_latitude(void) {
  return ground_latitude;
}

char Nmea::get_ground_latDir(void) {
  return ground_latDir;
}

float Nmea::get_ground_longitude(void) {
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