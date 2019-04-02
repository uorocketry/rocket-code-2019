#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <Adafruit_GPS.h>
#include "Yost.h"
MPL3115A2 myPressure;
SoftwareSerial mySerial(3, 2);
Adafruit_GPS myGPS(&mySerial);

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

boolean sensor_debug = true;

  typedef struct { //initialize structure
      float altitude;
      float velocity; // velocity to be implemented later (a_1 - a_0) / time_inbetween_readings
      float latitude;
      float longitude; 
      yost_imu rocket; 
  } state;
  
void setup() {
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output
  myPressure.begin(); // Get sensor online
  myGPS.begin(9600); // Get sensor online
 yost.begin();
 
  //Configure the sensor
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 

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
    if(sensor_debug){
    yost_imu rocket = {{0,0,0},{0,0,0}}; // place holder vaules until sensor is updated
    }
  typedef struct { //initialize structure
      float altitude;
      float velocity; // velocity to be implemented later (a_1 - a_0) / time_inbetween_readings
      float latitude;
      float longitude; 
      yost_imu rocket;
  } state;
  state init_state = {myPressure.readAltitude(), 20.0, myGPS.latitude, myGPS.longitude,{{0,0,0},{0,0,0}} }; // random velocity picked
  //state init_state = {myGPS.latitude, myGPS.longitude}; // random velocity picked

  
// function prototype
void update_state(state *); // pointer to struct
}

void loop() {
  state init_state;
  if(!sensor_debug){
    update_state(&init_state); // function call pass memory address of init_state
    DISPLAY_OUTPUT(&init_state); // print to serial port
  }
  else{
    read_dummy_sensors(&init_state);
    Serial.write(test_dummy_output(&init_state));
  }
   
}

void update_state(state *state_ptr){ // point to memory address (get contents of mem address)
  
  state_ptr->altitude = myPressure.readAltitude(); // the altitude field in the struct that state_ptr points to is updated to current Altitude
  state_ptr->latitude = myGPS.latitude; 
  state_ptr->longitude = myGPS.longitude; 
  

  // yost imu library 
    float *euler_orient = yost.read_orientation_euler(); // point to memory address of array
    
    state_ptr->rocket.e_orient.pitch = *euler_orient; // point (imu_ptr) to e_orient and access the pitch filed
    state_ptr->rocket.e_orient.yaw = *(euler_orient + 1);
    state_ptr->rocket.e_orient.roll = *(euler_orient + 2);

    // yost imu library 
    float *accel = yost.read_accel_filtered(); // point to memory address of array
    
    state_ptr->rocket.r_accel.x = *accel; // point (imu_ptr) to r_accel and access the x filed
    state_ptr->rocket.r_accel.y = *(accel + 1);
    state_ptr->rocket.r_accel.z = *(accel + 2);

}

void DISPLAY_OUTPUT(state *state_ptr){
  if(!sensor_debug) {
    Serial.print("Altitude(m):");
    Serial.print(state_ptr->altitude, 2);
    Serial.println();
    Serial.print("Latitude(Degrees):");
    Serial.print(state_ptr->latitude, 2);
    Serial.println();
    Serial.print("Longitude(Degrees):");
    Serial.print(state_ptr->longitude, 2); //2?
    Serial.println();
 

    Serial.print("Pitch: ");
    Serial.print(state_ptr->rocket.e_orient.pitch); 
    Serial.println();

    Serial.print("Yaw: ");
    Serial.print(state_ptr->rocket.e_orient.yaw); 
    Serial.println();

    Serial.print("Roll: ");
    Serial.print(state_ptr->rocket.e_orient.roll); 
    Serial.println();

    Serial.print(""); // \n
    Serial.println();

    Serial.print("x: ");
    Serial.print(state_ptr->rocket.r_accel.x);
    Serial.println();

    Serial.print("y: ");
    Serial.print(state_ptr->rocket.r_accel.y); 
    Serial.println();

    Serial.print("z: ");
    Serial.print(state_ptr->rocket.r_accel.z); 
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

void read_dummy_sensors(state *state_ptr) {

  while(!Serial.available()){
    
  }
  
  if(Serial.available()>0) {
    String data_from_python = Serial.readString();
    int commaIndex = data_from_python.indexOf(",");
    int secondCommaIndex = data_from_python.indexOf(",", commaIndex+1);
    int thirdCommaIndex = data_from_python.indexOf(",", secondCommaIndex+1);
    int fourthCommaIndex = data_from_python.indexOf(",", thirdCommaIndex+1);
    int fifthCommaIndex = data_from_python.indexOf(",", fourthCommaIndex+1);
    int sixthCommaIndex = data_from_python.indexOf(",", fifthCommaIndex+1);
    int seventhCommaIndex = data_from_python.indexOf(",", sixthCommaIndex+1);
    int eighthCommaIndex = data_from_python.indexOf(",", seventhCommaIndex+1);
    int ninthCommaIndex = data_from_python.indexOf(",", eighthCommaIndex+1);
    

    String firstValue = data_from_python.substring(0, commaIndex);
    String secondValue = data_from_python.substring(commaIndex+1, secondCommaIndex);
    String thirdValue = data_from_python.substring(secondCommaIndex+1, thirdCommaIndex);
    String fourthValue = data_from_python.substring(thirdCommaIndex+1, fourthCommaIndex);
    String fifthValue = data_from_python.substring(fourthCommaIndex+1, fifthCommaIndex);
    String sixthValue = data_from_python.substring(fifthCommaIndex+1, sixthCommaIndex);
    String seventhValue = data_from_python.substring(sixthCommaIndex+1, seventhCommaIndex);
    String eighthValue = data_from_python.substring(seventhCommaIndex+1, eighthCommaIndex);
    String ninthValue = data_from_python.substring(eighthCommaIndex+1, ninthCommaIndex);
    String tenthValue = data_from_python.substring(ninthCommaIndex+1);
    //////////////////Unfinished, results in segmentation fault.
    //Should be similair to this when finished
    
    state_ptr->altitude = firstValue.toFloat();
    state_ptr->velocity = secondValue.toFloat();
    state_ptr->latitude = thirdValue.toFloat();
    state_ptr->longitude = fourthValue.toFloat();
    /*
    state_ptr->rocket.e_orient.pitch = fifthValue.toFloat(); 
    state_ptr->rocket.e_orient.yaw = sixthValue.toFloat();
    state_ptr->rocket.e_orient.roll = seventhValue.toFloat();

    state_ptr->rocket.r_accel.x = eighthValue.toFloat(); 
    state_ptr->rocket.r_accel.y = ninthValue.toFloat();
    state_ptr->rocket.r_accel.z = tenthValue.toFloat();
    */
    float *euler_orient = yost.read_orientation_euler(); // point to memory address of array
    
    state_ptr->rocket.e_orient.pitch = *euler_orient; // point (imu_ptr) to e_orient and access the pitch filed
    state_ptr->rocket.e_orient.yaw = *(euler_orient + 1);
    state_ptr->rocket.e_orient.roll = *(euler_orient + 2);

    // yost imu library 
    float *accel = yost.read_accel_filtered(); // point to memory address of array
    
    state_ptr->rocket.r_accel.x = *accel; // point (imu_ptr) to r_accel and access the x filed
    state_ptr->rocket.r_accel.y = *(accel + 1);
    state_ptr->rocket.r_accel.z = *(accel + 2);
    
    
  }
}

int test_dummy_output(state *init_state) {
  if(init_state->altitude >=0 && init_state->altitude<=1000 && init_state->velocity <= 80 && init_state->velocity>=70) {
    return 1;
  }
  else if(init_state->altitude >=0 && init_state->altitude<=1000 && init_state->velocity <= 70 && init_state->velocity>=60) {
    return 2;
  }
  else if(init_state->altitude >=0 && init_state->altitude<=1000 && init_state->velocity <= 60 && init_state->velocity>=50) {
    return 3;
  }
  else if(init_state->altitude >=0 && init_state->altitude<=1000 && init_state->velocity <= 50 && init_state->velocity>=0) {
    return 4;
  }
  else if(init_state->altitude >=1001 && init_state->altitude<=2000 && init_state->velocity <= 80 && init_state->velocity>=70) {
    return 5;
  }
  else if(init_state->altitude >=1001 && init_state->altitude<=2000 && init_state->velocity <= 70 && init_state->velocity>=60) {
   return 6;
  }
  else if(init_state->altitude >=1001 && init_state->altitude<=2000 && init_state->velocity <= 60 && init_state->velocity>=50) {
    return 7;
  }
  else if(init_state->altitude >=1001 && init_state->altitude<=2000 && init_state->velocity <= 50 && init_state->velocity>=00) {
    return 8;
  }
  else if(init_state->altitude >=2001 && init_state->altitude<=3000 && init_state->velocity <= 80 && init_state->velocity>=70) {
    return 9;
  }
  else if(init_state->altitude >=2001 && init_state->altitude<=3000 && init_state->velocity <= 70 && init_state->velocity>=60) {
    return 10;
  }
  else if(init_state->altitude >=2001 && init_state->altitude<=3000 && init_state->velocity <= 60 && init_state->velocity>=50) {
    return 11;
  }
  else if(init_state->altitude >=2001 && init_state->altitude<=3000 && init_state->velocity <= 50 && init_state->velocity>=0) {
    return 12;
  }
}
