#include <SoftwareSerial.h>
SoftwareSerial mySerial(1, 2); // Arduino Nano Rx and TX pins

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Hello world!");

  mySerial.begin(57600);
  mySerial.println("Hello, world?");
}

void loop() {
  if (mySerial.available()) {
    Serial.print(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write("Hi");
  }
}
