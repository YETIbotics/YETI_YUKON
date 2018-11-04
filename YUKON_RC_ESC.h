#pragma once

class Adafruit_PWMServoDriver;

class YUKON_RC_ESC{
	
public:

	YUKON_RC_ESC(int channel, int pinpwm, bool reversed);
	YUKON_RC_ESC(int channel, Adafruit_PWMServoDriver *Refpwm, bool reversed);
	
	void Init();

	void SetMotorSpeed(float speed);

	void SetInputParameters(float InputMin, float InputMax, float InputDeadZone);

private:
	float ScrubInputWithParameters(float speed);

	Adafruit_PWMServoDriver *pwm;

	bool _isPWMBoard = false;

	int _pinPWM;
	bool _reverse;
	int _channel;

	float _prevSpeed = 1;
	float _inputDeadZone = 0;
	float _inputMin = -255;
	float _inputMax = 255;

};


