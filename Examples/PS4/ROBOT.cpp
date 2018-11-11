#include "ROBOT.h"

ROBOT::ROBOT(YETI_YUKON &rYukon) : Yukon(rYukon),
                                     DriveRight(_DriveRightPWM, &Yukon.PWM, _DriveRightDir, &Yukon.GPIO, _DriveRightReversed), 
                                     DriveLeft(_DriveLeftPWM, &Yukon.PWM, _DriveLeftDir, &Yukon.GPIO, _DriveLeftReversed), 
                                     LiftMotor(_LiftMotorPWM, &Yukon.PWM, _LiftMotorReversed), 
                                                                        Drive(*this),
                                    Lift(*this),
                                   Auton(*this),
                                    
                                    Btd(&Usb), PS4(&Btd) //, PAIR)
                                    
                                   
{
}

void ROBOT::Setup()
{
    Usb.Init();
    DriveRight.Init();
    DriveLeft.Init();
    LiftMotor.Init();
}

void ROBOT::Loop()
{
    Usb.Task();
        
         if (PS4.connected())
        {
            Drive.OISetSpeed(Yukon.PS4JoystickTo255(PS4.getAnalogHat(RightHatY), 10),Yukon.PS4JoystickTo255(PS4.getAnalogHat(LeftHatY), 10));
            Lift.OISetSpeed(PS4.getAnalogButton(R2) - PS4.getAnalogButton(L2));
        
            if (PS4.getButtonClick(LEFT))
                Auton.StartAuton("Auton1");
            if (PS4.getButtonClick(RIGHT))
                Auton.StartAuton("Auton2");

            if (PS4.getButtonClick(UP))
                Yukon.SetupWIFI();

            if (PS4.getButtonClick(PS))
                PS4.disconnect();
        }
        

    
        DriveRight.SetMotorSpeed(State.DriveRightSpeed);
        DriveLeft.SetMotorSpeed(State.DriveLeftSpeed);
        LiftMotor.SetMotorSpeed(State.LiftMotorSpeed);
}
