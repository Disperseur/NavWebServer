#ifndef ALARM_H
#define ALARM_H

//capteur pression
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "NMEA.h"
using namespace rtos;


#define BUZZER_PIN 69

void pressureAlarmThreadEntryPoint(void* arg);
void depthAlarmThreadEntryPoint(void* arg);

void buzzerAlarmWatcherEntryPoint(void* arg);
// void cb_pressure_alarm_reset(void);




#endif