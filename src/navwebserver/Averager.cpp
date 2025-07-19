#include "Averager.h"

Averager::Averager(unsigned long windowDurationSec)
  : count(0), window(windowDurationSec) {}

void Averager::addSample(const String& gpsTimeStr, float value) {
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
  if (count == 0) return 0.0;
  float sum = 0.0;
  for (int i = 0; i < count; ++i) {
    sum += samples[i].value;
  }
  return sum / count;
}

unsigned long Averager::timeStringToSeconds(const String& timeStr) const {
  if (timeStr.length() < 6) return 0;

  unsigned int h = timeStr.substring(0, 2).toInt();
  unsigned int m = timeStr.substring(2, 4).toInt();
  unsigned int s = timeStr.substring(4, 6).toInt();

  return h * 3600UL + m * 60UL + s;
}
