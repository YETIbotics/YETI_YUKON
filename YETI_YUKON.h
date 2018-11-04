#ifndef _YETI_YUKON_H_
#define _YETI_YUKON_H_

//WIFI and Wireless Deploy Libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ArduinoOTA.h>

//OLED Display Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_PWMServoDriver.h> //PWM Library
#include <Adafruit_MCP23017.h>       //GPIO Library
#include <Adafruit_MCP3008.h>        //ADC Library

#include "YUKON_MPU6050.h"

class YETI_YUKON
{

public:
  YETI_YUKON(const char *RobotName, const char *Password);

  Adafruit_SSD1306 OLED;

  Adafruit_PWMServoDriver PWM;
  Adafruit_MCP23017 GPIO;
  Adafruit_MCP3008 ADC;

  AsyncWebServer server;
  DNSServer dns;

  YUKON_MPU6050 GYRO;

  void Setup();
  void Loop();

private:
  void SetupOTA();
  const char *robotName = "";
  const char *password = "";
};

#endif /* _YETI_YUKON_H_ */