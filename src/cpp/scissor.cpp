#include <scissor.h>

void ScissorLift::start_loop(float max_speed){
	this->max_speed = max_speed;
}
void ScissorLift::set_target(int target_inches){ //How high want scissor lift to go
	std::cout<<"Set Target -- target_inches1: "<<target_inches<<std::endl;
	this->target_inches = target_inches;
	this->target_NU = input_height_function(target_inches);
	std::cout<<"Set Target -- target_inches2: "<<this->target_NU<<std::endl;
}
float ScissorLift::input_height_function(int target_inches){
		double travel_distance_for_scissor_lift = (20.5 - (20.5 * cos(asin(target_inches/(20.5*5)))));
		std::cout<<"Input Height Function -- travel_distance_for_scissor_lift: "<<travel_distance_for_scissor_lift<<std::endl;
		double actual_rotations_for_scissor_lift_scale = (one_rotation_encoder_counts * rotations_per_inch * travel_distance_for_scissor_lift);
		double input_height_NU = actual_rotations_for_scissor_lift_scale;
		if (target_inches < 0)
		{
			input_height_NU = -1 * input_height_NU;
		}
		return input_height_NU;
}
float ScissorLift::get_target_NU(){
	//std::cout<<"Get Target -- target_inches: "<<target_inches<<std::endl;
	return target_NU;
}
float ScissorLift::get_target_inches(){
	//std::cout<<"Get Target -- target_inches: "<<target_inches<<std::endl;
	return target_inches;
}
float ScissorLift::get_position(){
	//std::cout<<"Get Position -- get_position: "<<std::max(talon_right_enc->GetSelectedSensorPosition(0), talon_left_enc->GetSelectedSensorPosition(0))<<std::endl;
	return std::max(talon_right_enc->GetSelectedSensorPosition(0), talon_left_enc->GetSelectedSensorPosition(0));
}
void ScissorLift::run_loop(){
	sync();
		if(mode == 1)
		{
			talon_right_enc->Set(ControlMode::Position, (get_position() - 1000));
			talon_left_enc->Set(ControlMode::Position, (get_position() - 1000));
			std::cout<<" "<<std::endl;
			std::cout<<"\nGP - ECPL: "<<(get_position() - 1000)<<std::endl;
			std::cout<<"\nGet Position -- run_loop: "<<get_position()<<std::endl;
			std::cout<<" "<<std::endl;
			std::cout<<"I'm the debugging pig, debugging pig!"<<std::endl;
			std::cout<<"Run loop -- max_speed: "<<max_speed<<std::endl;
		}
		else if(mode == 2)
		{
			if (talon_right_enc->GetSelectedSensorPosition(0) >= 0 || talon_left_enc->GetSelectedSensorPosition(0) >= 0 )
			{
				talon_left_enc->Set(ControlMode::PercentOutput, 0);
				talon_right_enc->Set(ControlMode::PercentOutput, 0);
			}
			talon_right_enc->Set(ControlMode::Position, (get_position() + 1000));
			talon_left_enc->Set(ControlMode::Position, (get_position() + 1000));
			std::cout<<" "<<std::endl;
			std::cout<<"\nGP + ECPL: "<<(get_position() + 1000)<<std::endl;
			std::cout<<"\nGet Position -- run_loop: "<<get_position()<<std::endl;
			std::cout<<" "<<std::endl;
			std::cout<<"I'm the debugging pig, debugging pig Down!"<<std::endl;
			std::cout<<"Run loop -- max_speed: "<<max_speed<<std::endl;
		}
		std::cout<<"Pinking Pig"<<std::endl;
	
}
void ScissorLift::sync(){
	double Error = abs(abs(talon_right_enc->GetSelectedSensorPosition(0)) - abs(talon_left_enc->GetSelectedSensorPosition(0)));
	double ErrorPercentage = ((Error)/(std::min(talon_right_enc->GetSelectedSensorPosition(0), talon_left_enc->GetSelectedSensorPosition(0))));
	if (abs(talon_right_enc->GetSelectedSensorPosition(0)) > abs(talon_left_enc->GetSelectedSensorPosition(0)))
	{
		talon_right_enc->ConfigPeakOutputForward(max_speed * (1 - abs(ErrorPercentage)), 10);
		talon_right_enc->ConfigPeakOutputReverse(-1 * (max_speed * (1 - abs(ErrorPercentage))), 10);
		talon_left_enc->ConfigPeakOutputForward(max_speed, 10);
		talon_left_enc->ConfigPeakOutputReverse(-max_speed, 10);
	}
	else if (abs(talon_right_enc->GetSelectedSensorPosition(0)) < abs(talon_left_enc->GetSelectedSensorPosition(0)))
	{
		talon_left_enc->ConfigPeakOutputForward(max_speed * (1 - abs(ErrorPercentage)), 10);
		talon_left_enc->ConfigPeakOutputReverse(-1 * (max_speed * (1 - abs(ErrorPercentage))), 10);
		talon_right_enc->ConfigPeakOutputForward(max_speed, 10);
		talon_right_enc->ConfigPeakOutputReverse(-max_speed, 10);
	}
	else
	{
		talon_left_enc->ConfigPeakOutputForward(max_speed, 10);
		talon_left_enc->ConfigPeakOutputReverse(-max_speed, 10);
		talon_right_enc->ConfigPeakOutputForward(max_speed, 10);
		talon_right_enc->ConfigPeakOutputReverse(-max_speed, 10);
		std::cout<<"#Debugging pig of the year"<<std::endl;
	}
	/*
	talon_left_enc->ConfigPeakOutputForward(max_speed, 10);
	talon_left_enc->ConfigPeakOutputReverse(-max_speed, 10);
	talon_right_enc->ConfigPeakOutputForward(max_speed, 10);
	talon_right_enc->ConfigPeakOutputReverse(-max_speed, 10);
	std::cout<<"Left Velocity: "<<talon_left_enc->GetSelectedSensorVelocity(0)<<std::endl;
	std::cout<<"#Debugging pig of the year"<<std::endl;*/
}
void ScissorLift::stop_loop(){
	talon_left_enc->Set(ControlMode::PercentOutput, 0);
	talon_right_enc->Set(ControlMode::PercentOutput, 0);
}

