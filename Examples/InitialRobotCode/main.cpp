#include <Arduino.h>
#include <Wire.h>

#include <YETI_YUKON.h>
#include <ROBOT.h>

YETI_YUKON Yukon("TestUnit", "robotsrule");
ROBOT Robot(Yukon);

void setup()
{
    Serial.begin(115200);

    Wire.begin(4, 15, 400000);

    Yukon.Setup();
    Robot.Setup();
}

void loop()
{
    Yukon.Loop(); 
    Robot.Loop();

    delay(1);
}