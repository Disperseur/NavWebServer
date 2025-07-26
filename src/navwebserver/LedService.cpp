#include "LedService.h"



void ledThreadEntryPoint() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  while(1) {
    digitalWrite(LED_GREEN, LOW);
    ThisThread::sleep_for(500);
    digitalWrite(LED_GREEN, HIGH);
    ThisThread::sleep_for(500);
  }
}