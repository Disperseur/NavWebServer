#ifndef ALARM_H
#define ALARM_H

//capteur pression
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
using namespace rtos;




void pressure_alarm_reset(void);
void pressureAlarmThreadEntryPoint();


#endif