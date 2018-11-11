#ifndef ROBOT_h
#define ROBOT_h

#include <YETI_YUKON.h>
#include <STATE.h>

#include <PS4BT.h>
#include <usbhub.h>

#include <YUKON_CytronMD10.h>
#include <YUKON_RC_ESC.h>

#include "Subsystems\Drive.h"
#include "Subsystems\Lift.h"

#include "Autonomous\AUTONOMOUS.h"

class ROBOT
{
public:
  ROBOT(YETI_YUKON &rYukon);

  void Setup();
  void Loop();

  YETI_YUKON &Yukon;

  STATE State;

  //Motors
  YUKON_CytronMD10 DriveRight;
  YUKON_CytronMD10 DriveLeft;
  YUKON_RC_ESC LiftMotor;

  //Subsystems
  DRIVE Drive;
  LIFT Lift;
  AUTONOMOUS Auton;

  //USB Items
  USB Usb;

  BTD Btd;
  PS4BT PS4;

  //PIN Declarations
  static const int _DriveRightPWM = 0;
  static const int _DriveRightDir = 0;
  static const bool _DriveRightReversed = false;

  static const int _DriveLeftPWM = 1;
  static const int _DriveLeftDir = 1;
  static const bool _DriveLeftReversed = true;

  static const int _LiftMotorPWM = 2;
  static const bool _LiftMotorReversed = true;

  static const uint8_t _Button0 = 0;
  static const uint8_t _LEDBuiltIn = 25;
  static const uint8_t _AutonLightSensor = 0;

  static const uint16_t _AutonLightSensorThreshold = 512; //Value 0 - 1024

private:
  long _NextDisplayMillis = 0;
  long _NextMotorControllerWriteMillis = 0;
};

#endif