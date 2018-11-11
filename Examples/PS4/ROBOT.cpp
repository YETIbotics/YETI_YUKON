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
    //Read The Controller
    Usb.Task();

    if (PS4.connected())
    {
        Drive.OISetSpeed(Yukon.PS4JoystickTo255(PS4.getAnalogHat(RightHatY), 10), Yukon.PS4JoystickTo255(PS4.getAnalogHat(LeftHatY), 10));
        Lift.OISetSpeed(PS4.getAnalogButton(R2) - PS4.getAnalogButton(L2));

        if (PS4.getButtonClick(LEFT))
            Auton.QueuePrev();
        if (PS4.getButtonClick(RIGHT))
            Auton.QueueNext();
        if (PS4.getButtonClick(DOWN))
            Auton.ToggleArmed();

        if (PS4.getButtonClick(UP))
            Yukon.SetupWIFI();

        if (PS4.getButtonClick(X))
            Auton.LaunchQueued();

        if (PS4.getButtonClick(PS))
            PS4.disconnect();
    }

    //Read The Sensors
    uint16_t LightSensorVal = Yukon.ADC.readADC(_AutonLightSensor);
    State.AutonLightSensorActive = (LightSensorVal >= _AutonLightSensorThreshold);

    //Write To Motor Controllers
    if (_NextMotorControllerWriteMillis < millis())
    {
        _NextMotorControllerWriteMillis = millis() + 20;

        DriveRight.SetMotorSpeed(State.DriveRightSpeed);
        DriveLeft.SetMotorSpeed(State.DriveLeftSpeed);
        LiftMotor.SetMotorSpeed(State.LiftMotorSpeed);
    }

    //Write the Display
    if (_NextDisplayMillis < millis())
    {
        _NextDisplayMillis = millis() + 250;
        if (Auton.IsArmed())
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);

            Yukon.OLED.print("ARMED ");
            Yukon.OLED.println(LightSensorVal);
            Yukon.OLED.setTextSize(1);
            Yukon.OLED.print(Auton.QueuedProgramName());

            Yukon.OLED.display();
        }
        else if(Auton.QueuedProgramName() != "")
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);

            Yukon.OLED.print(Auton.QueuedProgramName());

            Yukon.OLED.display();
        }
        else
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);
            Yukon.OLED.println("YETI YUKON");
            Yukon.OLED.display();
        }
    }
}
