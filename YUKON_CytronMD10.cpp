
#include "YUKON_CytronMD10.h"
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MCP23017.h>

YUKON_CytronMD10::YUKON_CytronMD10(int channel, int pinpwm, int pindir, bool reversed)
{
	_pinPWM = pinpwm;
	_pinDIR = pindir;
	_reverse = reversed;
	_channel = channel;

	pinMode(_pinDIR, OUTPUT);
	pinMode(_pinPWM, OUTPUT);

	ledcSetup(_channel, 20000, 8);
	ledcAttachPin(_pinPWM, _channel);
}

YUKON_CytronMD10::YUKON_CytronMD10(int channel, Adafruit_PWMServoDriver *Refpwm, int pindir, Adafruit_MCP23017 *Refgpio, bool reversed)
{
	pwm = Refpwm;
	_isPWMBoard = true;

	gpio = Refgpio;
	_isGPIOBoard = true;

	_pinDIR = pindir;
	_reverse = reversed;
	_channel = channel;
}

void YUKON_CytronMD10::Init()
{
	if (_isPWMBoard)
	{
		pwm->setPWM(_channel, 0, 0);
	}

	if (_isGPIOBoard)
	{
		gpio->pinMode(_pinDIR, OUTPUT);
	}
}

void YUKON_CytronMD10::SetInputParameters(float InputMin, float InputMax, float InputDeadZone)
{
	_inputMin = InputMin;
	_inputMax = InputMax;
	_inputDeadZone = InputDeadZone;
}

float YUKON_CytronMD10::ScrubInputWithParameters(float speed)
{
	if (speed > _inputDeadZone)
	{
		//Going backwards
		speed = map(speed, _inputDeadZone, _inputMax, 0, 255);
	}
	else if (speed < -_inputDeadZone)
	{
		//going forwards
		speed = map(speed, -_inputDeadZone, _inputMin, 0, -255);
	}
	else
	{
		speed = 0;
	}
	return speed;
}

void YUKON_CytronMD10::SetMotorSpeed(float speed)
{
	speed = ScrubInputWithParameters(speed);

	if (_reverse)
		speed = speed * -1;

	if (_prevSpeed != speed)
	{
		_prevSpeed = speed;
		
		if (speed > 0)
		{
			dir = 1;
		}
		else
		{
			dir = 0;
		}

		if (_isPWMBoard)
		{
			pwm->setPWM(_channel, 0, abs(map(speed, 0, 255, 0, 4095)));
		}
		else
		{
			ledcWrite(_channel, abs(speed));
		}

		if (_isGPIOBoard)
		{
			gpio->digitalWrite(_pinDIR, dir);
		}
		else
		{
			digitalWrite(_pinDIR, dir);
		}
		
	}
}
