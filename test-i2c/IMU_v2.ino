//Component: Yost 3-Space™ LX Embedded part number TSS-LX

#include "Yost.h"
#include <Wire.h>

#define TRUE 1
#define FALSE 0

Yost yost;

unsigned long previousMillis = 0;
const long interval = 500;

//global variables
boolean sensor_debug = false;

void setup() {

  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output

  yost.begin();
  //sw_reset(); // software reset
  //axis_w_tare();// Tare with current orientation

}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
  previousMillis = currentMillis;
  //read_imu();
  read_euler();
  //read_accel();
  }
}




void read_imu() {
  if(!sensor_debug) {
    float temp = yost.getTemperature();
    Serial.print("Yost Temperature(c): ");
    Serial.print(temp);
    Serial.println();
    delay(500);
  }
  else{
     if (Serial.available() > 0) {
         int x = Serial.read();
         Serial.write(x);
        }
  }
}

void read_euler() {
  if(!sensor_debug) {

    float *euler_orient = yost.read_orientation_euler(); // point to memory address of array
    Serial.print("Pitch: ");
    Serial.print(*euler_orient); // get contents of memory addess (1st element of array)
    Serial.println();

    Serial.print("Yaw: ");
    Serial.print(*(euler_orient + 1)); // get contents of memory addess (2nd element of array)
    Serial.println();

    Serial.print("Roll: ");
    Serial.print(*(euler_orient + 2)); // get contents of memory addess (3rd element of array)
    Serial.println();


    Serial.print(""); // \n
    Serial.println();


  }
  else{
     if (Serial.available() > 0) {
         int x = Serial.read();
         Serial.write(x);
        }
  }
}


void read_accel() {
  if(!sensor_debug) {
    float *accel = yost.read_accel_filtered(); // point to memory address of array
    Serial.print("x: ");
    Serial.print(*accel); // get contents of memory addess (1st element of array)
    Serial.println();

    Serial.print("y: ");
    Serial.print(*(accel + 1)); // get contents of memory addess (2nd element of array)
    Serial.println();

    Serial.print("z: ");
    Serial.print(*(accel + 2)); // get contents of memory addess (3rd element of array)
    Serial.println();

    Serial.print(""); // \n
    Serial.println();

  }
  else{
     if (Serial.available() > 0) {
         int x = Serial.read();
         Serial.write(x);
        }
  }
}

void axis_w_tare(){
   if(!sensor_debug) {
    yost.set_axis_directions_with_tare();
    Serial.print("Axis tared");
    Serial.println();
  }
  else{
     if (Serial.available() > 0) {
         int x = Serial.read();
         Serial.write(x);
        }
  }
}

void sw_reset(){
   if(!sensor_debug) {
    Serial.print("Yost 3-Space LX Embedded Reset");
    Serial.println();
    yost.software_reset();
  }
  else{
     if (Serial.available() > 0) {
         int x = Serial.read();
         Serial.write(x);
        }
  }
}
