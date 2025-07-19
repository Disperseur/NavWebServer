#ifndef AVERAGER_H
#define AVERAGER_H

#include <Arduino.h>

class Averager {
public:
  Averager(unsigned long windowDurationSec);
  void addSample(const String& gpsTimeStr, float value);
  float getAverage() const;

private:
  struct Sample {
    unsigned long timeSec;
    float value;
  };

  static const int MAX_SAMPLES = 300;
  Sample samples[MAX_SAMPLES];
  int count;
  unsigned long window;

  unsigned long timeStringToSeconds(const String& timeStr) const;
};

#endif
