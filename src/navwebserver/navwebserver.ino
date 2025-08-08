#include "NMEA.h"
#include "NMEAServer.h"
#include "config.h"
#include "Alarm.h"
#include "LedService.h"
#include <mbed.h>
#include "Callback.h"

using namespace rtos;



Thread ledThread(osPriorityLow);
Thread serverThread(osPriorityNormal);
Thread parserThread(osPriorityRealtime6);

Thread pressureAlarmThread(osPriorityLow);
Thread depthAlarmThread(osPriorityLow);
Thread buzzerAlarmWatcherThread(osPriorityLow);

static Nmea bateau;

#ifdef DEBUG_MCUTEMP
mbed::AnalogIn mcuADCTemp(ADC_TEMP); // pour la mesure de la temperature MCU
#endif



void setup() {
  Serial.begin(460800);
  Serial1.begin(460800);
  Serial2.begin(460800);

  ledThread.start(                mbed::callback(ledThreadEntryPoint,           &bateau)); // status led start
  parserThread.start(             mbed::callback(parserThreadEntryPoint,        &bateau));

  ThisThread::sleep_for(1000);

  serverThread.start(             mbed::callback(serverThreadEntryPoint,        &bateau));

  pressureAlarmThread.start(      mbed::callback(pressureAlarmThreadEntryPoint, &bateau));
  depthAlarmThread.start(         mbed::callback(depthAlarmThreadEntryPoint,    &bateau));

  buzzerAlarmWatcherThread.start( mbed::callback(buzzerAlarmWatcherEntryPoint,  &bateau));
}

void loop() {
#ifdef DEBUG_MCUTEMP
  int mcuTemp = __HAL_ADC_CALC_TEMPERATURE (3300, mcuADCTemp.read_u16(), ADC_RESOLUTION_16B);

  Serial2.print("MCU Temp : ");
  Serial2.print(mcuTemp);
  Serial2.println(" *C");
#endif

  ThisThread::sleep_for(1000);
}


