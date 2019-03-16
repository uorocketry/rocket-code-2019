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
  YOST_REGISTER_TEMP           =           (0x2B),
};


class Yost
{
  public:
    Yost();
    boolean begin(TwoWire *twoWire = &Wire);

    float getTemperature();

  private:
   TwoWire *_i2c;

   union temp_tag {
     byte tempArray[4];
     float tempFloat;
   } temp;

};

#endif
