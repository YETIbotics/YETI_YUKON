
#include "YUKON_DBH12V.h"
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MCP23017.h>

YUKON_DBH12V::YUKON_DBH12V(int pwm1, int pwm2, Adafruit_PWMServoDriver *Refpwm, bool reversed)
{
	_pwm1 = pwm1;
	_pwm2 = pwm2;
	_reverse = reversed;
	pwm = Refpwm;
}

YUKON_DBH12V::YUKON_DBH12V(int espChannel1, int pinpwm1, int espChannel2, int pinpwm2, bool reversed)
{
	isESPPins = true;

	_pwm1 = pinpwm1;
	_espChannel1 = espChannel1;

	_pwm2 = pinpwm2;
	_espChannel2 = espChannel2;

	_reverse = reversed;
}

void YUKON_DBH12V::Init()
{
	if (isESPPins)
	{
		pinMode(_pwm1, OUTPUT);
		ledcSetup(_espChannel1, 20000, 8);
		ledcAttachPin(_pwm1, _espChannel1);

		pinMode(_pwm2, OUTPUT);
		ledcSetup(_espChannel2, 20000, 8);
		ledcAttachPin(_pwm2, _espChannel2);
	}
	else
	{
		pwm->setPWM(_pwm1, 0, 0);
		pwm->setPWM(_pwm2, 0, 0);
	}
}

void YUKON_DBH12V::SetMotorSpeed(float speed)
{
	if (_reverse)
		speed = speed * -1;

	if (_prevSpeed != speed)
	{
		_prevSpeed = speed;

		if (speed > 0)
		{
			if (isESPPins)
			{
				ledcWrite(_espChannel1, 0);
				ledcWrite(_espChannel2, abs(speed));
			}
			else
			{
				pwm->setPWM(_pwm1, 0, 0);
				pwm->setPWM(_pwm2, 0, abs(map(speed, 0, 255, 0, 4095)));
			}
		}
		else
		{
			if (isESPPins)
			{
				ledcWrite(_espChannel1, abs(speed));
				ledcWrite(_espChannel2, 0);
			}
			else
			{
				pwm->setPWM(_pwm1, 0, abs(map(speed, 0, 255, 0, 4095)));
				pwm->setPWM(_pwm2, 0, 0);
			}
		}
	}
}
