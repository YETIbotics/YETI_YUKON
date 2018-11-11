#ifndef STATE_h
#define STATE_h

#include <Arduino.h>

class STATE
{

  public:
	STATE();

    volatile int16_t DriveRightSpeed = 0;
    volatile int16_t DriveLeftSpeed = 0;
    volatile int16_t LiftMotorSpeed = 0;

  private:

};

#endif