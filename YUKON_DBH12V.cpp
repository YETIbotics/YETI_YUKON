
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

// void playSound(uint16_t freq, uint16_t duration_msec, uint8_t  amplitude)
// {
//   uint32_t n,k,period_usec,duration_usec;

//   period_usec = 1000000L / freq;
//   duration_usec = 1000 * (uint32_t) duration_msec;
//   k = duration_usec / period_usec;

//   motorSpeed (amplitude, amplitude);

//   for (n = 0; n < k; n++)
//   {
//     motorDirection (DIR_FORWARD, DIR_FORWARD);
//     delayMicroseconds(period_usec/2);
//     motorDirection (DIR_REVERSE, DIR_REVERSE);
//     delayMicroseconds(period_usec/2);
//   }
//   motorSpeed (0, 0);
// }

void YUKON_DBH12V::Init()
{

	pwm->setPWM(_pwm1, 0, 0);
	pwm->setPWM(_pwm2, 0, 0);

	// for(int i = 0; i < 1000; i++)
	// {
	// 	pwm->setPWM(_pwm1, 0, 0);
	// 	pwm->setPWM(_pwm2, 0, 4095);
	// 	delayMicroseconds(1);
	// 	pwm->setPWM(_pwm1, 0, 4095);
	// 	pwm->setPWM(_pwm2, 0, 0);
	// 	delayMicroseconds(1);
	// }

	// pwm->setPWM(_pwm1, 0, 0);
	// pwm->setPWM(_pwm2, 0, 0);
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
			pwm->setPWM(_pwm1, 0, 0);
			pwm->setPWM(_pwm2, 0, abs(map(speed, 0, 255, 0, 4095)));
		}
		else
		{
			pwm->setPWM(_pwm1, 0, abs(map(speed, 0, 255, 0, 4095)));
			pwm->setPWM(_pwm2, 0, 0);
		}

		// if (_isPWMBoard)
		// {
		// 	pwm->setPWM(_channel, 0, abs(map(speed, 0, 255, 0, 4095)));
		// }
		// else
		// {
		// 	ledcWrite(_channel, abs(speed));
		// }

		// if (_isGPIOBoard)
		// {
		// 	gpio->digitalWrite(_pinDIR, dir);
		// }
		// else
		// {
		// 	digitalWrite(_pinDIR, dir);
		// }
	}
}
