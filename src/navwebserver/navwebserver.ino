#include "NMEA.h"



Nmea bateau;
String nmea_msg("");

void setup() {
  Serial.begin(460800); // debug com
  Serial1.begin(460800); // data serial port on RX0
}

void loop() {
  while(Serial1.available()) {
    nmea_msg = Serial1.readStringUntil('\n');
    bateau.parse(nmea_msg);

#ifdef DEBUG
    Serial.println(nmea_msg);
#endif
  }

  
  bateau.printData();

  delay(100);
}
