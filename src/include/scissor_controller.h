#ifndef SCISSOR_CONTROLLER_H
#define SCISSOR_CONTROLLER_H

#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <iostream>
#include <Joystick.h>
#include <cmath>
#include <settings.h>
#include <scissor.h>
#include <valuecontrol.h>

class ScissorController
{
	public:
		ScissorController(Joystick *joy, ScissorLift *scissorlift) : joy(joy), scissorlift(scissorlift) {};;
		void run_loop();

	private:
		ScissorLift *scissorlift;
		Joystick *joy;
		
		const float manual_step_length =  3;
};

#endif