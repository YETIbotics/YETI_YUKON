#pragma once

#include <Arduino.h>

class YUKON_AUTON
{

public:
	YUKON_AUTON();

	void StartAuton(String AutonName);
	void LaunchQueued();
	void QueueNext();
	void QueuePrev();
	void ToggleArmed();
	bool IsArmed();
	String QueuedProgramName();
	
	String _RunningAuton = "";
	String _QueuedAuton = "";
	uint8_t _QueuedProgramNumber = 0;
	bool _AutonARMED = false;
  uint8_t _MaxProgramNumber = 0;

private:
};
