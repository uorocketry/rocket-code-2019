
// Serial mySerial(0, 1);
int ctr;

// Air data rate in one byte form, max value is 255
// Current value: 4
#define rx_air_speed 4
byte radio_array[rx_air_speed];

void setup() {
  Serial.begin(57600);
  ctr = 0;
}

void radio_send ( byte data ) {
  Serial.write(data);
  delay(10);
}

void loop() {

  for (int i = 0; i < rx_air_speed; i++ ){
    radio_array[i] = i+1;
    radio_send(radio_array[i]);
  }
 
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
