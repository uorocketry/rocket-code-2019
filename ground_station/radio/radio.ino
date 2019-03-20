
// Serial mySerial(0, 1);
int ctr;

void setup() {
  Serial.begin(57600);
  Serial.write("Hello, world!");
  delay(500);
  Serial.write("\t- From Tx");
  ctr = 0;
}

void loop() {
  if (Serial.available() && ctr < 10) {
    Serial.write("Hi ");
    Serial.write(ctr);
    Serial.write("\r\n");
    delay(1000);
    ctr++;
  }
}
