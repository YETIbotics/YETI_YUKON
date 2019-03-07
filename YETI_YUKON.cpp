#include "YETI_YUKON.h"

YETI_YUKON::YETI_YUKON(const char *RobotName, const char *Password) : OLED(0x3C, 4, 15, GEOMETRY_128_64)
{
    robotName = RobotName;
    password = Password;
}

void YETI_YUKON::Setup()
{
    GYRO.Setup();

    PWM.begin();
    PWM.setPWMFreq(250);

    GPIO.begin(); // use default address 0

    ADC.begin(2);

    //Turn on the OLED
    pinMode(16, OUTPUT);
    digitalWrite(16, LOW); // turn D2 low to reset OLED
    delay(50);
    digitalWrite(16, HIGH); // while OLED is running, must set D2 in high

    //Initialize the Display
    OLED.init();
    OLED.flipScreenVertically();

    OLED.drawXbm(0, 0, 128, 64, yeti_logo);
    OLED.display();

    pinMode(25, OUTPUT);
    digitalWrite(25, LOW);

    _lastWatchdogPat = millis();

    preferences.begin("yukon", true);
    WifiEnabled = preferences.getBool("setupwifi", false);
    preferences.end();

    if (WifiEnabled)
        SetupWIFI();
}

void YETI_YUKON::Enable()
{
    _Disabled = false;
}

void YETI_YUKON::Disable()
{
    _Disabled = true;
}

bool YETI_YUKON::IsDisabled()
{
    return _Disabled;
}

void YETI_YUKON::ToggleWIFI()
{
    preferences.begin("yukon", false);
    WifiEnabled = preferences.getBool("setupwifi", false);
    WifiEnabled = !WifiEnabled;
    preferences.putBool("setupwifi", WifiEnabled);
    preferences.end();

    ESP.restart();
    // if (!WifiEnabled)
    //    ESP.restart();
    // else
    //     SetupWIFI();
}

void YETI_YUKON::SetupWIFI()
{
    digitalWrite(25, HIGH);

    _watchdogPaused = true;
    _lastWatchdogPat = millis();

    //WiFi.begin("FreePublicWIFI");
    //delay(2000);
    //if (WiFi.status() == WL_CONNECTED)
    if (wifiMulti.run() == WL_CONNECTED)
    {
        OLED.clear();
        OLED.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
        OLED.setFont(ArialMT_Plain_16);
        OLED.drawString(OLED.getWidth() / 2, OLED.getHeight() / 2, "Ready for OTA:\n" + WiFi.localIP().toString());
        OLED.display();

        OLED.setTextAlignment(TEXT_ALIGN_LEFT);

        delay(1000);
        SetupOTA();
    }
    else
    {
        //if connection failed, change preferences to not reconnect on next reboot.
        ToggleWIFI();
        digitalWrite(25, LOW);
    }

    _lastWatchdogPat = millis();
    _watchdogPaused = false;
}

void YETI_YUKON::GeneralTask()
{

    ArduinoOTA.handle();

    _ChAVolts = ADC.readADC(7);
    _ChAVolts = ADC.readADC(6);
    _ChAVolts = ADC.readADC(5);

    //delay(1);
}

float YETI_YUKON::ChAVolts()
{
    return _ChAVolts;
}
float YETI_YUKON::ChBVolts()
{
    return _ChBVolts;
}
float YETI_YUKON::ChCVolts()
{
    return _ChCVolts;
}

void YETI_YUKON::PatTheDog()
{
    _lastWatchdogPat = millis();
}
void YETI_YUKON::EnableWatchdog()
{
    _lastWatchdogPat = millis();
    _watchdogEnabled = true;
}
void YETI_YUKON::DisableWatchdog()
{
    _watchdogEnabled = false;
}

void YETI_YUKON::WatchdogLoop()
{
    if (_watchdogEnabled && !_watchdogPaused && millis() - _lastWatchdogPat > _watchdogBite)
    {
        Serial.println("Watchdog Bite! Restarting.");
        Serial.print(millis() - _lastWatchdogPat);
        Serial.print(" > ");
        Serial.println(_watchdogBite);
        ESP.restart();
    }
}

unsigned long StartTime = 0;
void YETI_YUKON::SetupOTA()
{
    ArduinoOTA.setHostname(robotName);
    ArduinoOTA.setPassword(password);
    ArduinoOTA
        .onStart([this]() {
            DisableWatchdog();

            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            StartTime = millis();

            OLED.clear();
            OLED.setFont(ArialMT_Plain_10);
            OLED.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
            OLED.drawString(OLED.getWidth() / 2, OLED.getHeight() / 2 - 10, "OTA Update");
            OLED.display();

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            //KillAllTasks();
        })
        .onEnd([]() {
            // OLED.clear();
            // OLED.setFont(ArialMT_Plain_10);
            // OLED.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
            // OLED.drawString(OLED.getWidth()/2, OLED.getHeight()/2, "Restart");
            // OLED.display();
        })
        .onProgress([this](unsigned int progress, unsigned int total) {
            OLED.clear();

            OLED.drawProgressBar(4, 44, 120, 12, progress / (total / 100));

            OLED.setTextAlignment(TEXT_ALIGN_RIGHT);
            OLED.setFont(ArialMT_Plain_24);
            OLED.drawString(124, 8, String((millis() - StartTime) / 1000) + "s");

            OLED.setTextAlignment(TEXT_ALIGN_LEFT);
            OLED.setFont(ArialMT_Plain_24);
            OLED.drawString(4, 8, String(progress * 100 / total) + "%");

            OLED.display();
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
                Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)
                Serial.println("End Failed");
        });

    ArduinoOTA.begin();
}

int16_t YETI_YUKON::ScrubInputWithParameters(int16_t JoystickValue, int16_t Deadzone, int16_t InputMin, int16_t InputMax, bool reverseInput)
{
    int16_t _inZero = abs(InputMax + InputMin) / 2;

    if (JoystickValue > _inZero + Deadzone)
    {
        JoystickValue = map(JoystickValue, _inZero + Deadzone, InputMax, 0, 255);
    }
    else if (JoystickValue < _inZero - Deadzone)
    {
        JoystickValue = map(JoystickValue, _inZero - Deadzone, InputMin, 0, -255);
    }
    else
    {
        JoystickValue = 0;
    }
    if (reverseInput)
        return JoystickValue * -1;

    return JoystickValue;
}

//Value Mappers
int16_t YETI_YUKON::JoystickTo255(int16_t JoystickValue, int16_t Deadzone)
{
    return ScrubInputWithParameters(JoystickValue, Deadzone, 0, 255, true);
}
int16_t YETI_YUKON::XBOXJoystickTo255(int16_t JoystickValue, int16_t Deadzone)
{
    return ScrubInputWithParameters(JoystickValue, Deadzone, -32767, 32767, false);
}