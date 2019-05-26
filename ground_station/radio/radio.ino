
// Serial mySerial(0, 1);
int ctr;

void setup() {
  Serial.begin(57600);
  ctr = 0;
  delay(5000);
}

// Copy this part into main Arduino code
// ==========================================================================================
// Air data rate in one byte form, max value is 255
// Current value: 64
#define rx_air_speed 64
byte radio_array[rx_air_speed];

void radio_send ( byte data[rx_air_speed] ) {
  for ( int i = 0; i < rx_air_speed; i++) {
    Serial.write(data[i]);
    delay(200);
  }
}
// ==========================================================================================


void loop() {
  
  for (int i = 0; i < rx_air_speed; i++ ){
    radio_array[i] = i+1;
    radio_send(radio_array[i]);
  }

  if (Serial.available() && ctr < 20) {
    if (ctr == -1) {
        delay(500);
        Serial.write("Hello, world!\r\n");
        Serial.write("\t- From Tx\r\n");
    }
    Serial.write(ctr);
    //Serial.write("\r\n");
//    Serial.flush();
    delay(1000);
    ctr++;
  }
}
