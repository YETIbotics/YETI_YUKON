#pragma once

class Adafruit_PWMServoDriver;
class Adafruit_MCP23017;

class YUKON_DBH12V{
	
public:
	 YUKON_DBH12V(int pwm1, int pwm2, Adafruit_PWMServoDriver *Refpwm, bool reversed);
	 YUKON_DBH12V(int espChannel1, int pinpwm1, int espChannel2, int pinpwm2, bool reversed);

	void Init();

	void SetMotorSpeed(float speed);

private:
	bool isESPPins = false;
	int _espChannel1;
	int _espChannel2;

	Adafruit_PWMServoDriver *pwm;

	int _pwm1;
	int _pwm2; 
	bool _reverse;

	float _prevSpeed = 1;
};


