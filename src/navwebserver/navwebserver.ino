void setup() {
  Serial.begin(460800); // debug com
  Serial1.begin(460800); // data serial port on RX0
}

void loop() {
  while(Serial1.available()) {
    char c = Serial1.read();
    Serial.print(c);
  }

  delay(500);
}
