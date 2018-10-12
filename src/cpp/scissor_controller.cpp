#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <iostream>
#include <Joystick.h>
#include <cmath>
#include <settings.h>
#include <scissor.h>
#include <valuecontrol.h>
#include <scissor_controller.h>


void ScissorController::run_loop(){	
	
	bool throttle_up_modified = joy->GetRawAxis(3) > throttle_cutoff;
	bool throttle_down_modified = joy->GetRawAxis(2) > throttle_cutoff;

	if(throttle_up_modified && !throttle_down_modified){
		scissorlift->mode = 1;
		scissorlift->set_target(scissorlift->get_target_inches() - manual_step_length);
				if ((scissorlift->get_target_inches() - manual_step_length) == 0)
				{
					scissorlift->set_target(scissorlift->get_target_inches() - manual_step_length - 0.5);
				}
		scissorlift->run_loop();

	}
	else if(throttle_down_modified && !throttle_up_modified){
		scissorlift->mode = 2;
		scissorlift->set_target(scissorlift->get_target_inches() + manual_step_length);
				if ((scissorlift->get_target_inches() + manual_step_length) == 0)
				{
					scissorlift->set_target(scissorlift->get_target_inches() + manual_step_length + 0.5);
				}
		scissorlift->run_loop();

	}
	else{
		scissorlift->set_target(scissorlift->input_height_function(scissorlift->get_position()));
	}

	
	std::cout<<"Current Position: "<<scissorlift->get_position()<<"\tCurrent Target: "<<scissorlift->get_target_inches()<<std::endl;
}