#ifndef ROBOT_h
#define ROBOT_h

#include <YETI_YUKON.h>
#include <STATE.h>
#include <XBOXRECV.h>
#include <YUKON_RC_ESC.h>
#include <YUKON_CytronMD10.h>

#include <Subsystems\Lift.h>
#include <Subsystems\Drive.h>

class ROBOT
{
  public:
	ROBOT(YETI_YUKON &rYukon);

	void Setup();
	void Loop();

	YETI_YUKON &Yukon;

	STATE State;

	//USB Items
	USB Usb;
	XBOXRECV Xbox;

	//Motors
	YUKON_RC_ESC LiftMotor;
	YUKON_CytronMD10 DriveLeftMotor;
	YUKON_CytronMD10 DriveRightMotor;

	//Subsystems
	LIFT Lift;
	DRIVE Drive;

	//PIN Declarations
	static const int _LeftEnc0 = 14;
	static const int _LeftEnc1 = 17;
	static const int _RightEnc0 = 21;
	static const int _RightEnc1 = 22;
	static const int _LiftEnc0 = 26;
	static const int _LiftEnc1 = 27;

	static const int _LiftPWM = 2;
	static const int _RightPWM = 0;
	static const int _RightDIR = 0;
	static const int _LeftPWM = 1;
	static const int _LeftDIR = 1;

	static const int _Button0 = 0;
	static const int _LEDBuiltIn = 25;

  private:
 
};

#endif