/*#include <scissor-ish.h>

void ScissorLift::start_loop(float max_speed){
	max_speed = max_speed;
}
void ScissorLift::set_target(int target_inches){ //How high want scissor lift to go
	target_inches = input_height_function(target_inches);
	target_inches = target_inches;
}
float ScissorLift::input_height_function(int target_inches){
		double travel_distance_for_scissor_lift = (20.5 - (20.5 * cos(asin(target_inches/(20.5*5)))));
		double actual_rotations_for_scissor_lift_scale = (one_rotation_encoder_counts * rotations_per_inch * travel_distance_for_scissor_lift);
		double input_height_NU = actual_rotations_for_scissor_lift_scale;
		return input_height_NU;
}
float ScissorLift::get_target(){
	return target_inches;
}
float ScissorLift::get_position(){
	return std::max(talon_right_enc->GetSelectedSensorPosition(0), talon_left_enc->GetSelectedSensorPosition(0));
}
void ScissorLift::run_loop(){
	sync();
	if (abs(get_position()) < abs(get_target()))
	{
		if(get_position() < get_target()){
			talon_right_enc->Set(ControlMode::Position, get_position() + encoder_counts_per_loop);
			talon_left_enc->Set(ControlMode::Position, get_position() + encoder_counts_per_loop);
	}
	else if(get_position() < get_target()){
			talon_right_enc->Set(ControlMode::Position, get_position() + encoder_counts_per_loop);
			talon_left_enc->Set(ControlMode::Position, get_position() + encoder_counts_per_loop);
	}
}
}
void ScissorLift::sync(){
	double Error = abs(abs(talon_right_enc->GetSelectedSensorPosition(0)) - abs(talon_left_enc->GetSelectedSensorPosition(0)));
	double ErrorPercentage = ((Error)/(std::min(talon_right_enc->GetSelectedSensorPosition(0), talon_left_enc->GetSelectedSensorPosition(0))));
	if (abs(talon_right_enc->GetSelectedSensorPosition(0)) > abs(talon_left_enc->GetSelectedSensorPosition(0)))
	{
		talon_right_enc->ConfigPeakOutputForward(max_speed * (1 - abs(ErrorPercentage)), 10);
		talon_right_enc->ConfigPeakOutputReverse(-1 * (max_speed * (1 - abs(ErrorPercentage))), 10);
		talon_left_enc->ConfigPeakOutputForward(max_speed, 10);
		talon_left_enc->ConfigPeakOutputReverse(max_speed, 10);
	}
	else if (abs(talon_right_enc->GetSelectedSensorPosition(0)) < abs(talon_left_enc->GetSelectedSensorPosition(0)))
	{
		talon_left_enc->ConfigPeakOutputForward(max_speed * (1 - abs(ErrorPercentage)), 10);
		talon_left_enc->ConfigPeakOutputReverse(-1 * (max_speed * (1 - abs(ErrorPercentage))), 10);
		talon_right_enc->ConfigPeakOutputForward(max_speed, 10);
		talon_right_enc->ConfigPeakOutputReverse(max_speed, 10);
	}
	
}
void ScissorLift::stop_loop(){
	talon_left_enc->Set(ControlMode::PercentOutput, 0);
	talon_right_enc->Set(ControlMode::PercentOutput, 0);
}
*/