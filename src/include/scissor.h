#ifndef SCISSOR_LIFT_H
#define SCISSOR_LIFT_H

#include <ctre/Phoenix.h>
#include <WPILib.h>
#include <iostream>
#include <cmath>

class ScissorLift
{
public:
	ScissorLift(TalonSRX *talon_left_enc, TalonSRX *talon_right_enc) : talon_right_enc(talon_right_enc), talon_left_enc(talon_left_enc) {
	
	};
		int mode; 	
		void start_loop(float max_speed);
		float get_position();
		float get_target_NU();
		float get_target_inches();
		void set_target(int target_inches);
		void run_loop();
		void stop_loop();
		float input_height_function(int target_inches);

private:
	TalonSRX *talon_left_enc, *talon_right_enc;
	
	
	void sync();
	int one_rotation_encoder_counts = 4096;
	int rotations_per_inch = 5;
	float max_speed;
	int target_inches = 0;
	int encoder_counts_per_loop = 20;
	int target_NU;
};

#endif
