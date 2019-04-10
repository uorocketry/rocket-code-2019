//Component: Yost 3-Space™ LX Embedded part number TSS-LX

#include "Yost.h"
#include <Wire.h>

Yost yost;

    typedef struct{ //initialize struct
    float pitch;
    float yaw;
    float roll;
    }euler_angles;

    typedef struct{
    float x;
    float y;
    float z;
    }accel;

    typedef struct{
    euler_angles e_orient;
    accel r_accel;
    }yost_imu; // user defined data type

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

      typedef struct{ //initialize struct
    float pitch;
    float yaw;
    float roll;
    }euler_angles;

    typedef struct{
    float x;
    float y;
    float z;
    }accel;

    typedef struct{
    int rot_array[9];
    }rot;

    typedef struct{
    euler_angles e_orient;
    accel r_accel;
    rot r_matrix;
    }yost_imu; // user defined data type

    yost_imu rocket = {{0,0,0},{0,0,0}}; // place holder vaules until sensor is updated

}

void loop() {

  yost_imu rocket;
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
  previousMillis = currentMillis;
  //read_imu();
  update_yost(&rocket);
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


void update_yost(yost_imu *imu_ptr){ // point to memory address (get contents of mem address)
  if(!sensor_debug) {
    
    // yost imu library 
    float *euler_orient = yost.read_orientation_euler(); // point to memory address of array
    
    imu_ptr->e_orient.pitch = *euler_orient; // point (imu_ptr) to e_orient and access the pitch filed
    imu_ptr->e_orient.yaw = *(euler_orient + 1);
    imu_ptr->e_orient.roll = *(euler_orient + 2);

    // yost imu library 
    float *accel = yost.read_accel_filtered(); // point to memory address of array
    
    imu_ptr->r_accel.x = *accel; // point (imu_ptr) to r_accel and access the x filed
    imu_ptr->r_accel.y = *(accel + 1);
    imu_ptr->r_accel.z = *(accel + 2);

    Serial.print("Pitch: ");
    Serial.print(imu_ptr->e_orient.pitch); 
    Serial.println();

    Serial.print("Yaw: ");
    Serial.print(imu_ptr->e_orient.yaw); 
    Serial.println();

    Serial.print("Roll: ");
    Serial.print(imu_ptr->e_orient.roll); 
    Serial.println();

    Serial.print(""); // \n
    Serial.println();


    Serial.print("x: ");
    Serial.print(imu_ptr->r_accel.x); // get contents of memory addess (1st element of array)
    Serial.println();

    Serial.print("y: ");
    Serial.print(imu_ptr->r_accel.y); // get contents of memory addess (2nd element of array)
    Serial.println();

    Serial.print("z: ");
    Serial.print(imu_ptr->r_accel.z); // get contents of memory addess (3rd element of array)
    Serial.println();

    Serial.print(""); // \n
    Serial.println();


    
  } else{
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
