
#include "ROBOT.h"

ROBOT::ROBOT(YETI_YUKON &rYukon) : Yukon(rYukon), 
			Xbox(&Usb), 
			LiftMotor(_LiftPWM, &Yukon.PWM, false), 
			DriveLeftMotor(_LeftPWM, &Yukon.PWM, _LeftDIR, &Yukon.GPIO, true), 
			DriveRightMotor(_RightPWM, &Yukon.PWM, _RightDIR, &Yukon.GPIO, false),
			Lift(*this),
			Drive(*this)
{
}

void ROBOT::Setup()
{
	Usb.Init();

	DriveRightMotor.Init();
	DriveRightMotor.SetInputParameters(-32767, 32767, 7500);

	DriveLeftMotor.Init();
	DriveLeftMotor.SetInputParameters(-32767, 32767, 7500);

	LiftMotor.Init();
}

void ROBOT::Loop()
{
	Usb.Task();

	Usb.Task();
	if (Xbox.XboxReceiverConnected)
	{
		for (uint8_t i = 0; i < 4; i++)
		{
			if (Xbox.Xbox360Connected[i])
			{
				State.LiftSpeed = Xbox.getButtonPress(R2, i) - Xbox.getButtonPress(L2, i);
				State.LeftSpeed = Xbox.getAnalogHat(LeftHatY, i);
				State.RightSpeed = Xbox.getAnalogHat(RightHatY, i);
			}
		}
	}

	Lift.Loop();
	Drive.Loop();
}
