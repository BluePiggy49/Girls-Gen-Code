#ifndef CROSS_THE_LINE_H
#define CROSS_THE_LINE_H

#include "WPILib.h"
#include "ctre/Phoenix.h"
#include <iostream>
#include <math.h>
#include <Timer.h>
#include <scissor.h>

class CrossTheLine
{
public:
	CrossTheLine(TalonSRX *talon_left_enc, TalonSRX *talon_right_enc, TalonSRX *intake1, TalonSRX *intake2, ScissorLift *scissorlift)
	: drive_talon_right_enc(drive_talon_right_enc), drive_talon_left_enc(drive_talon_left_enc), intake1(intake1), intake2(intake2), scissorlift(scissorlift) {
	};
	bool Start = 1;
	int mode = 1;
	void Start_Autonomous();
	void Us_Left();


private:
	TalonSRX *drive_talon_left_enc, *drive_talon_right_enc, *intake1, *intake2;
	ScissorLift *scissorlift;
	Timer timer;
	void Run_Autonomous();
	void Them_Left();
		
};

#endif
