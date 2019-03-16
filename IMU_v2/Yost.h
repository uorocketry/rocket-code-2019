//Component: Yost 3-Space™ LX Embedded part number TSS-LX

/*
  Yost.h - Library for interfacing with the Yost labs 3-space LX.
  Created by uORocketry 2019.
  Released into the public domain.
*/
#ifndef Yost_h
#define Yost_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
 #include <TinyWireM.h>
#else
 #include <Wire.h>
#endif


#define YOST_ADDRESS                       (0x77)    ///< default I2C address 1100000
#define YOST_I2C_COMMAND                   (0xEE)
#define YOST_STATUS_COMMAND                (0x41)
#define YOST_WRITE_COMMAND                 (0x42)
#define YOST_READ_COMMAND                  (0x43)


/**************************************************************************/
/*!
    @brief  MPL3115A2 registers
*/
/**************************************************************************/
enum {
  YOST_REGISTER_TEMP           =           (0x2B), // Read temperature (Celsius) {Return Len: 4 bytes} {Return Data Details: float}
  YOST_REGISTER_EULER          =           (0x01), // Read filtered, tared orientation(Euler Angles) {Return Len 12 bytes} {Return Data Details: Euler Angles(float x3)}
  YOST_REGISTER_ACCEL          =           (0x27), // Read accelerometer {Return Len: 12 bytes} {Return Data Details: Vector(float x3)}
  YOST_REGISTER_AXIS_W_TARE    =           (0x60), // Tare with current orientation {Return Len: 0 bytes} {Data Details: N/A}
  YOST_REGISTER_SW_RESET       =           (0xE2), // Software reset. Resets the sensor {Return Len: N/A} {Data Details: N/A}
  YOST_REGISTER_SET_AXIS       =           (0x74) // Set axis directions {Return Len: 1 byte} {Data Details: Axis direction byte}
};


class Yost
{
  public:
    Yost();
    boolean begin(TwoWire *twoWire = &Wire);

    float getTemperature();
    float *read_orientation_euler();
    float *read_accel_filtered();
    void set_axis_directions_with_tare();
    void software_reset();

    byte  set_axis();


  private:
   TwoWire *_i2c;

   union temp_tag {
     byte tempArray[4]; // {Return Len: 4 bytes} {Return Data Details: float}
     float tempFloat;
   } temp;

   union euler_tag {
     byte eulerArray[4]; // {Return Len 12 bytes} {Return Data Details: Euler Angles(float x3)}
     float eulerFloat;
    } euler;

   //float angle_euler[3]; // store each float (every 4 bytes) into an index of the xyz_euler array

   union accel_tag {
     byte accelArray[4]; // Read accelerometer {Return Len: 12 bytes} {Return Data Details: Vector(float x3)}
     float accelFloat;
    } accel;

   float xyz_accel[3]; // store each float (every 4 bytes) into an index of the xyz_euler array

};

#endif
