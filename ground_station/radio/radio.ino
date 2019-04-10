
// Serial mySerial(0, 1);
int ctr;

void setup() {
  Serial.begin(57600);
  ctr = 0;
}

void loop() {
  if (Serial.available() && ctr < 20) {
    if (ctr == 0) {
        delay(500);
        Serial.write("Hello, world!\r\n");
        Serial.write("\t- From Tx\r\n");
    }
    Serial.write("Hi ");
    Serial.write(ctr);
    Serial.write("\r\n");
    delay(1000);
    ctr++;
  }
}
