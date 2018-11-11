#include "ROBOT.h"

ROBOT::ROBOT(YETI_YUKON &rYukon) : Yukon(rYukon),
                                     DriveRight(_DriveRightPWM, &Yukon.PWM, _DriveRightDir, &Yukon.GPIO, _DriveRightReversed), 
                                     DriveLeft(_DriveLeftPWM, &Yukon.PWM, _DriveLeftDir, &Yukon.GPIO, _DriveLeftReversed), 
                                     LiftMotor(_LiftMotorPWM, &Yukon.PWM, _LiftMotorReversed), 
                                                                        Drive(*this),
                                    Lift(*this),
                                   Auton(*this),
                                    
                                    Xbox(&Usb)
                                    
                                   
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
        
         if (Xbox.XboxReceiverConnected)
        {
            for (uint8_t i = 0; i < 4; i++)
            {
                if (Xbox.Xbox360Connected[i])
                {
                    Drive.OISetSpeed(Yukon.XBOXJoystickTo255(Xbox.getAnalogHat(RightHatY, i), 7500),Yukon.XBOXJoystickTo255(Xbox.getAnalogHat(LeftHatY, i), 7500));
                    Lift.OISetSpeed(Xbox.getButtonPress(R2, i) - Xbox.getButtonPress(L2, i));
        
                    if (Xbox.getButtonClick(LEFT))
                        Auton.StartAuton("Auton1");
                    if (Xbox.getButtonClick(RIGHT))
                        Auton.StartAuton("Auton2");

                    if (Xbox.getButtonClick(UP))
                        Yukon.SetupWIFI();
                }
            }
        }
        

    
        DriveRight.SetMotorSpeed(State.DriveRightSpeed);
        DriveLeft.SetMotorSpeed(State.DriveLeftSpeed);
        LiftMotor.SetMotorSpeed(State.LiftMotorSpeed);
}
