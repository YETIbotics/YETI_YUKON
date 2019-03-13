#include "YUKON_COMMAND.h"

YUKON_COMMAND::YUKON_COMMAND()
{
}

bool YUKON_COMMAND::CmdIsRunning()
{
    return _CmdIsRunning;
}
String YUKON_COMMAND::CmdName()
{
    return _CmdName;
}
int16_t YUKON_COMMAND::CmdSetPoint()
{
    return _CmdSetPoint;
}
uint8_t YUKON_COMMAND::CmdPercentComplete()
{
    return _CmdPercentComplete;
}

float YUKON_COMMAND::CmdClicksRemaining()
{
    return _CmdSetPoint - _CurrentPoint;
}
bool YUKON_COMMAND::CmdStart(String CmdName, long Timeout)
{
    if (_CmdIsRunning)
        return false;

    _CmdName = CmdName;
    _CmdStartPoint = millis();
    _CmdSetPoint = Timeout + millis();
    _CmdIsRunning = true;
    _CmdTimeout = millis() + Timeout + 500;

    return true;
}


int16_t YUKON_COMMAND::BurnoutControl(int16_t DesiredSpeed, int16_t PrevSpeed)
{
     //if we are crossing the treshold of 0, then set prev speed to zero.
    if((DesiredSpeed > 0 && PrevSpeed <= 0) || (DesiredSpeed < 0 && PrevSpeed >= 0))
        PrevSpeed = 0;

    int16_t AbsDesiredSpeed = abs(DesiredSpeed);
    int16_t direction = (AbsDesiredSpeed != 0 ? (DesiredSpeed / AbsDesiredSpeed) : 1); //get direction
    int16_t AbsPrevSpeed = abs(PrevSpeed);
    int16_t AbsMaxSpeed =  AbsPrevSpeed + BurnoutIncrement; //(1.25 * AbsPrevSpeed) + 1;

    if(AbsDesiredSpeed > AbsMaxSpeed && AbsMaxSpeed <= 255)
    {
        return AbsMaxSpeed * direction;
    }

    return DesiredSpeed;

}

bool YUKON_COMMAND::CmdStart(String CmdName, int16_t StartPoint, int16_t SetPoint, long Timeout)
{
    if (_CmdIsRunning)
        return false;

    _CmdName = CmdName;
    _CmdStartPoint = StartPoint;
    _CmdSetPoint = SetPoint;
    _CmdIsRunning = true;
    _CmdTimeout = millis() + Timeout;
    _CmdPercentComplete = 0;

    return true;
}
void YUKON_COMMAND::CmdDone()
{
    _CmdName = "";
    _CmdStartPoint = 0;
    _CmdSetPoint = 0;
    _CmdIsRunning = false;
    _CmdTimeout = 0;
}
bool YUKON_COMMAND::CmdTimedOut()
{
    return _CmdTimedOut;
}

bool YUKON_COMMAND::CmdUpdatePercent(int16_t CurrentPoint)
{
    if (millis() > _CmdTimeout)
    {
        _CmdTimedOut = true;
        CmdDone();
        return false;
    }

    _CurrentPoint = CurrentPoint;
    _CmdPercentComplete = (abs((CurrentPoint - _CmdStartPoint)) * 100) / abs(_CmdSetPoint - _CmdStartPoint);

    if (_CmdPercentComplete >= 100)
    {
        _CmdPercentComplete = 100;
        CmdDone();
        return false;
    }

    return true;
}

int16_t YUKON_COMMAND::CalcControlLoop(int16_t inputSpeed)
{
    if (_SlowDown != 0)
    {
        if (abs(CmdClicksRemaining()) <= _SlowDown)
        {
            //What percent through the slowdown are we?
            float slowDownPercent = 1.0 * abs(CmdClicksRemaining()) / _SlowDown;

            //Cal
            float desiredMotorSpeed = (slowDownPercent * (255 - SlowdownIncrement)) + SlowdownIncrement;

            //make sure we don't go negative.
            if(desiredMotorSpeed < 0)
                desiredMotorSpeed = 0;

            //Generate a +1 or -1 based on direction of input speed
            int16_t direction = (inputSpeed != 0 ? (inputSpeed / abs(inputSpeed)) : 1);

            int16_t tmpDriveSpeed = (desiredMotorSpeed * direction);

            if (abs(tmpDriveSpeed) > abs(inputSpeed))
                tmpDriveSpeed = inputSpeed;

            return tmpDriveSpeed;
        }
    }


    return inputSpeed;

}