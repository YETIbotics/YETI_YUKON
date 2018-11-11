#include <Arduino.h>
#include <Wire.h>

#include <YETI_YUKON.h>
#include <ROBOT.h>

YETI_YUKON Yukon("YOURROBOTNAME", "99a94d7a-1582-4fb1-827d-69dbf13ea1df");
ROBOT Robot(Yukon);

void DriveTask(void *pvParameters);
void LiftTask(void *pvParameters);
void AutonTask(void *pvParameters);

void setup()
{
    Serial.begin(115200);

    Wire.begin(4, 15, 400000);

    Yukon.Setup();
    Robot.Setup();

    Serial.print("MainLoop: Executing on core ");
    Serial.println(xPortGetCoreID());

    xTaskCreatePinnedToCore(DriveTask, "DriveTask", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(LiftTask, "LiftTask", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(AutonTask, "AutonTask", 10000, NULL, 1, NULL, 1);
}

void loop()
{
    Yukon.Loop();
    Robot.Loop();

    delay(1);
    yield();
}

void DriveTask(void *pvParameters)
{
    Serial.print("DriveTask: Executing on core ");
    Serial.println(xPortGetCoreID());

    while (true)
    {
        Robot.Drive.Loop();
    }
}

void LiftTask(void *pvParameters)
{
    Serial.print("LiftTask: Executing on core ");
    Serial.println(xPortGetCoreID());

    while (true)
    {
        Robot.Lift.Loop();
    }
}

void AutonTask(void *pvParameters)
{
    Serial.print("AutonTask: Executing on core ");
    Serial.println(xPortGetCoreID());

    while (true)
    {
        Robot.Auton.Loop();
    }
}