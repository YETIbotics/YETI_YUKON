#pragma once

#include <Arduino.h>

    class ROBOT;

    class AUTONOMOUS
    {

    public:

    AUTONOMOUS(ROBOT &refRobot);
    void StartAuton(String AutonName);
    void Loop();


    private:
    ROBOT &Robot;
    String _RunningAuton = "";
    void ExecAuton1();
    void ExecAuton2();
    void ExecAuton3();
    void ExecAuton4();

    };


