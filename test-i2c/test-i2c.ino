 //imports
#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include "Yost.h"

//Create an instance of the object
MPL3115A2 myPressure;
Yost yost;

//global variables
boolean sensor_debug = false;

//setup
void setup() {

  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output

  myPressure.begin(); // Get sensor online
  yost.begin();

  //Configure the sensor
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags
}

void loop() {
  read_altimeter();
  read_imu();
}


void read_imu() {
  if(!sensor_debug) {
    float temp = yost.getTemperature();
    Serial.print("Yost Temperature(c): ");
    Serial.print(temp);
    Serial.println();
  }
  else{
     if (Serial.available() > 0) {
         int x = Serial.read();
         Serial.write(x);
        }
  }
}

void read_altimeter() {
  if(!sensor_debug) {
      float altitude = myPressure.readAltitude();
      Serial.print("Altitude(m):");
      Serial.print(altitude, 2);

      //float altitude = myPressure.readAltitudeFt();
      //Serial.print(" Altitude(ft):");
      //Serial.print(altitude, 2);

      //float pressure = myPressure.readPressure();
      //Serial.print("Pressure(Pa):");
      //Serial.print(pressure, 2);

      float temperature = myPressure.readTemp();
      Serial.print(" Temp(c):");
      Serial.print(temperature, 2);

      //float temperature = myPressure.readTempF();
      //Serial.print(" Temp(f):");
      //Serial.print(temperature, 2);

      Serial.println();
  }
  else{
     if (Serial.available() > 0) {
         int x = Serial.read();
         Serial.write(x);
        }
  }
}



void read_temp_c() {
  Wire.beginTransmission(0x77); // transmit to device #8
  Wire.write(0xEE);        // sends five bytes
  Wire.write(0x42);        // sends one byte
  Wire.write(0x2B);

  Wire.write(0xEE);        // sends five bytes
  Wire.write(0x43);
  Wire.endTransmission();    // stop transmitting

  Wire.requestFrom(0x77, 4);    // request 6 bytes from slave device #8

  int i = 0;
  union u_tag {
    byte speedArray[4];
    float speedLong;
  } u;




  while (Wire.available() && i<4) { // slave may send less than requested
    u.speedArray[3-i] = Wire.read(); // receive a byte as character
    i++;
  }
  Serial.println(""); // linefeed
  Serial.print("Yost Temp(c): ");
  Serial.println(u.speedLong);
  delay(500);

}

void getStatus() {
  Wire.beginTransmission(0x77); // transmit to device #8
  Wire.write(0xEE);        // sends five bytes
  Wire.write(0x41);        // sends one byte
  Wire.endTransmission();    // stop transmitting

  delay(500);

  Wire.beginTransmission(0x77); // transmit to device #8
  Wire.write(0xEE);        // sends five bytes
  Wire.write(0x43);
  Wire.endTransmission();    // stop transmitting

  Wire.requestFrom(0x77, 1);    // request 6 bytes from slave device #8


  int status = 0;

  while (Wire.available()) { // slave may send less than requested
    status = Wire.read(); // receive a byte as character
  }
  Serial.println(""); // linefeed
  Serial.print("Yost Status: ");
  Serial.println(status, BIN);
  delay(500);

}

