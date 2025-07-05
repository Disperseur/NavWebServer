#ifndef NMEA_H
#define NMEA_H

#include "GPRMC.h"
#include "SDDBT.h"
#include "WIMWV.h"
#include "WIMTW.h"
#include "VWVHW.h"


class Nmea
{
public:  
  int parse(String nmea);


  int set_ground_time(String t);
  int set_ground_date(String d);
  int set_wind_sensorStatus(char s);
  int set_ground_sensorStatus(char s);
  int set_ground_latitude(String lat); // latitude maintenant formatee
  int set_ground_latDir(char latD);
  int set_ground_longitude(String lon);
  int set_ground_longDir(char lonD);
  int set_ground_speedKts(float s);
  int set_ground_course(float c);
  int set_water_depthMeters(float d);
  int set_water_speedKnots(float s);
  int set_water_temperatureCelsius(float t);
  int set_wind_angle(float a);
  int set_wind_angleReference(char ar);
  int set_wind_speedKts(float s);
  int set_wind_speedUnit(char u);

  unsigned long get_running_time(void);
  String get_ground_time(void);
  String get_ground_date(void);
  char get_wind_sensorStatus(void);
  char get_ground_sensorStatus(void);
  String get_ground_latitude(void);
  char get_ground_latDir(void);
  String get_ground_longitude(void);
  char get_ground_longDir(void);
  float get_ground_speedKts(void);
  float get_ground_course(void);
  float get_water_depthMeters(void);
  float get_water_speedKnots(void);
  float get_water_temperatureCelsius(void);
  float get_wind_angle(void);
  char get_wind_angleReference(void);
  float get_wind_speedKts(void);
  char get_wind_speedUnit(void);

  

private:
  unsigned long running_time;

  String  ground_time;
  String  ground_date;

  char    wind_sensorStatus;
  char    ground_sensorStatus; //GPS status

  String  ground_latitude;
  char    ground_latDir;
  String  ground_longitude;
  char    ground_longDir;
  float   ground_speedKts;
  float   ground_course;

  float   water_depthMeters;
  float   water_speedKnots;
  float   water_temperatureCelsius;

  float   wind_angle;
  char    wind_angleReference;
  float   wind_speedKts;

};



#endif