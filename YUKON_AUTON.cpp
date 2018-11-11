#include "YUKON_AUTON.h"

YUKON_AUTON::YUKON_AUTON()
{
}

void YUKON_AUTON::StartAuton(String AutonName)
{
    _RunningAuton = AutonName;
}

void YUKON_AUTON::LaunchQueued()
{
    if (_QueuedAuton != "")
    {
        _RunningAuton = _QueuedAuton;
        _QueuedAuton = "";
        _AutonARMED = false;
    }
}
bool YUKON_AUTON::IsArmed()
{
    return _AutonARMED;
}
String YUKON_AUTON::QueuedProgramName()
{
    return _QueuedAuton;
}

void YUKON_AUTON::QueueNext()
{
    if (_QueuedProgramNumber == _MaxProgramNumber)
    {
        _QueuedProgramNumber = 0;
        _QueuedAuton = "";
    }
    else
    {
        _QueuedProgramNumber++;
        _QueuedAuton = "Auton" + String(_QueuedProgramNumber);
    }
}
void YUKON_AUTON::QueuePrev()
{
    if (_QueuedProgramNumber == 0)
    {
        _QueuedProgramNumber = _MaxProgramNumber;
        _QueuedAuton = "Auton" + String(_MaxProgramNumber);
    }
    else
    {
        _QueuedProgramNumber--;
        if (_QueuedProgramNumber > 0)
            _QueuedAuton = "Auton" + String(_QueuedProgramNumber);
        else
            _QueuedAuton = "";
    }
}
void YUKON_AUTON::ToggleArmed()
{
    _AutonARMED = !_AutonARMED;
}