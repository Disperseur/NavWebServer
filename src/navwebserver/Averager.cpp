#include "Averager.h"

Averager::Averager(unsigned long windowDurationSec)
  : count(0), window(windowDurationSec) {}

void Averager::addSample(const String& gpsTimeStr, float value) {
  // Serial.println("Adding sample...");

  unsigned long currentTime = timeStringToSeconds(gpsTimeStr);
  if (currentTime == 0) return;

  // Supprimer les anciens échantillons
  int validCount = 0;
  for (int i = 0; i < count; ++i) {
    if (currentTime - samples[i].timeSec <= window) {
      samples[validCount++] = samples[i];
    }
  }
  count = validCount;

  // Ajouter le nouveau
  if (count < MAX_SAMPLES) {
    samples[count++] = { currentTime, value };
  }
}

float Averager::getAverage() const {
  return avg;
}

int Averager::computeAverage() {
  if (count == 0) {
    avg = 0;
    return 0;
  }

  float sum = 0.0;

  Serial.print("Samples : [");

  for (int i = 0; i < count; ++i) {
    sum += samples[i].value;
    Serial.print(samples[i].value);
    Serial.print(" ");
  }

  Serial.println("]");

  avg = sum / count;
  //return sum / count;
}

unsigned long Averager::timeStringToSeconds(const String& timeStr) {
  // Serial.println("Calculating time string to seconds ...");

  if (timeStr.length() < 5) return 0;

  unsigned int h = timeStr.substring(0, 2).toInt();
  unsigned int m = timeStr.substring(3, 5).toInt();
  // unsigned int s = timeStr.substring(4, 6).toInt();

  // Serial.print("Calculated time : ");
  // Serial.print(h * 3600UL + m * 60UL);
  // Serial.println(" s");

  return h * 3600UL + m * 60UL; // + s;
}
