/*
#ifndef SCISSOR_LIFT_ISH_H
#define SCISSOR_LIFT_ISH_H

#include <ctre/Phoenix.h>
#include <WPILib.h>
#include <iostream>

class ScissorLift
{
public:
	ScissorLift(TalonSRX *talon_left_enc, TalonSRX *talon_right_enc) : talon_right_enc(talon_right_enc), talon_left_enc(talon_left_enc) {
	
	};
		void start_loop(float max_speed);
		float get_position();
		float get_target();
		void set_target(int target_inches);
		void run_loop();
		void stop_loop();

private:
	TalonSRX *talon_left_enc, *talon_right_enc;
	
	float input_height_function(int target_inches);
	void sync();
	int one_rotation_encoder_counts = 4096;
	int rotations_per_inch = 5;
	float max_speed;
	int target_inches;
	int encoder_counts_per_loop= 20;
};

#endif
*/