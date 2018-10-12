#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <iostream>
#include <Joystick.h>
#include <cmath>
#include <settings.h>
#include <scissor.h>
#include <intake.h>
#include <scissor_controller.h>
#include <PID.h>
#include <drivebase.h>
#include <Solenoid.h>
#include <pneumatic.h>

using namespace frc;

class Robot: public IterativeRobot {
	TalonSRX *drive_talon_left_enc,*drive_talon_left_noenc,*drive_talon_right_enc,*drive_talon_right_noenc, *scissorlift_talon_left,*scissorlift_talon_right, *climber_talon_noenc, *intake_talon_left,*intake_talon_right, *climber_talon_enc;
	ScissorLift *scissorlift;
	Joystick *joy;
	Intake *intake;
	DigitalInput *limitswitch_scissorlift_left,*limitswitch_scissorlift_right;
	ScissorController *joystick_scissor_lift;
	PID *pid;
	Solenoid *solenoid_1, *solenoid_2;
	DriveBase *drivebase;
	Pneumatic *pneumatic;
	int mode;

    void RobotInit() {
	//define talons
		std::cout<<"Fresher auto!!!"<<std::endl;
		drive_talon_left_enc = new TalonSRX(7);
		drive_talon_left_enc->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0,talon_timeout_ms);
		drive_talon_left_noenc = new TalonSRX(3);
		drive_talon_left_noenc->Set(ControlMode::Follower, 7);
		drive_talon_right_enc = new TalonSRX(4);
		drive_talon_right_enc->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0,talon_timeout_ms);
		drive_talon_right_noenc = new TalonSRX(5);
		drive_talon_right_noenc->Set(ControlMode::Follower, 4);

	//scissor
		scissorlift_talon_left = new TalonSRX(1);
		scissorlift_talon_right = new TalonSRX(8);

	//climber
		climber_talon_noenc = new TalonSRX(num_climber_talon_noenc);
		climber_talon_enc = new TalonSRX(num_climber_talon_enc);

	//intake
		intake_talon_left = new TalonSRX(num_intake_talon_left);
		intake_talon_right = new TalonSRX(num_intake_talon_right);
	
	//joystick
		joy = new Joystick(0);
	
	//scissor lift
		scissorlift = new ScissorLift(scissorlift_talon_left, scissorlift_talon_right);

	//intake
		intake = new Intake(intake_talon_right, intake_talon_left, joy);

	//joystick scissor lift
		joystick_scissor_lift = new ScissorController(joy, scissorlift);

	//PID	
		drive_talon_left_enc->Config_kF(0, 0, 10);
		drive_talon_left_enc->Config_kP(0, 1.58, 10);
		drive_talon_left_enc->Config_kI(0, 0, 10);
		drive_talon_left_enc->Config_kD(0, 42, 10);
		drive_talon_left_enc->SetSensorPhase(true);

		drive_talon_right_enc->Config_kF(0, 0, talon_timeout_ms);
		drive_talon_right_enc->Config_kP(0, 0.52, talon_timeout_ms);
		drive_talon_right_enc->Config_kI(0, 0, talon_timeout_ms);
		drive_talon_right_enc->Config_kD(0, 6, talon_timeout_ms);
		drive_talon_right_enc->SetSensorPhase(true);

		scissorlift_talon_left->Config_kF(0, 0, talon_timeout_ms);
		scissorlift_talon_left->Config_kP(0, 1, talon_timeout_ms);
		scissorlift_talon_left->Config_kI(0, 0, talon_timeout_ms);
		scissorlift_talon_left->Config_kD(0, 10, talon_timeout_ms);

		scissorlift_talon_right->Config_kF(0, 0, talon_timeout_ms);
		scissorlift_talon_right->Config_kP(0, 1, talon_timeout_ms);
		scissorlift_talon_right->Config_kI(0, 0, talon_timeout_ms);
		scissorlift_talon_right->Config_kD(0, 10, talon_timeout_ms);

	//DriveBase
		drivebase = new DriveBase(drive_talon_left_enc, drive_talon_left_noenc, drive_talon_right_enc, drive_talon_right_noenc, joy, intake_talon_left, intake_talon_right);	 

	//ScissorLift Zeroing
		scissorlift_talon_left->SetSelectedSensorPosition(0, 0, 10);
    	scissorlift_talon_right->SetSelectedSensorPosition(0, 0, 10);
	}

    void AutonomousInit() {
    	mode = 1;
    	drive_talon_left_enc->SetSelectedSensorPosition(0, 0, 10);
		drive_talon_right_enc->SetSelectedSensorPosition(0, 0, 10);
		drive_talon_left_enc->ConfigPeakOutputForward(1, 10);
		drive_talon_left_enc->ConfigPeakOutputReverse(-1, 10);
		drive_talon_left_enc->SetSensorPhase(true);
		drive_talon_right_enc->SetSensorPhase(true);
		drive_talon_right_enc->ConfigPeakOutputForward(1, 10);
		drive_talon_right_enc->ConfigPeakOutputReverse(-1, 10);
	}

	void sync(){
		double Error = abs(abs(drive_talon_right_enc->GetSelectedSensorPosition(0)) - abs(drive_talon_left_enc->GetSelectedSensorPosition(0)));
		double ErrorPercentage = ((Error)/(std::min(drive_talon_right_enc->GetSelectedSensorPosition(0), drive_talon_left_enc->GetSelectedSensorPosition(0))));
		float max_speed = 1;
		if (abs(drive_talon_right_enc->GetSelectedSensorPosition(0)) > abs(drive_talon_left_enc->GetSelectedSensorPosition(0)))
		{
			drive_talon_right_enc->ConfigPeakOutputForward(max_speed * (1 - abs(ErrorPercentage)), 10);
			drive_talon_right_enc->ConfigPeakOutputReverse(-1 * (max_speed * (1 - abs(ErrorPercentage))), 10);
			drive_talon_left_enc->ConfigPeakOutputForward(max_speed, 10);
			drive_talon_left_enc->ConfigPeakOutputReverse(-max_speed, 10);
		}
		else if (abs(drive_talon_right_enc->GetSelectedSensorPosition(0)) < abs(drive_talon_left_enc->GetSelectedSensorPosition(0)))
		{
			drive_talon_left_enc->ConfigPeakOutputForward(max_speed * (1 - abs(ErrorPercentage)), 10);
			drive_talon_left_enc->ConfigPeakOutputReverse(-1 * (max_speed * (1 - abs(ErrorPercentage))), 10);
			drive_talon_right_enc->ConfigPeakOutputForward(max_speed, 10);
			drive_talon_right_enc->ConfigPeakOutputReverse(-max_speed, 10);
		}
		else
		{
			drive_talon_left_enc->ConfigPeakOutputForward(max_speed, 10);
			drive_talon_left_enc->ConfigPeakOutputReverse(-max_speed, 10);
			drive_talon_right_enc->ConfigPeakOutputForward(max_speed, 10);
			drive_talon_right_enc->ConfigPeakOutputReverse(-max_speed, 10);
		}
	}
	float get_position(){
		return std::max(abs(drive_talon_right_enc->GetSelectedSensorPosition(0)), abs(drive_talon_left_enc->GetSelectedSensorPosition(0)));
	}
    void AutonomousPeriodic() {
    	/*drive_talon_left_enc->Config_kF(0, std::atof(SmartDashboard::GetString("DB/String 0", "0.0").c_str()), 10);
		drive_talon_left_enc->Config_kP(0, std::atof(SmartDashboard::GetString("DB/String 1", "0.0").c_str()), 10);
		drive_talon_left_enc->Config_kI(0, std::atof(SmartDashboard::GetString("DB/String 2", "0.0").c_str()), 10);
		drive_talon_left_enc->Config_kD(0, std::atof(SmartDashboard::GetString("DB/String 3", "0.0").c_str()), 10); 
    	drive_talon_right_enc->Config_kF(0, std::atof(SmartDashboard::GetString("DB/String 5", "0.0").c_str()), 10);
		drive_talon_right_enc->Config_kP(0, std::atof(SmartDashboard::GetString("DB/String 6", "0.0").c_str()), 10);
		drive_talon_right_enc->Config_kI(0, std::atof(SmartDashboard::GetString("DB/String 7", "0.0").c_str()), 10);
		drive_talon_right_enc->Config_kD(0, std::atof(SmartDashboard::GetString("DB/String 8", "0.0").c_str()), 10); */
		
		if (mode == 1)
		{
			std::cout<<"Mode 1"<<std::endl;
			sync();
			if (drive_talon_right_enc->GetSelectedSensorPosition(0) < 22000 || drive_talon_left_enc->GetSelectedSensorPosition(0) > -22000)
			{
				drive_talon_right_enc->Set(ControlMode::Position, (get_position() + 1000));
				drive_talon_left_enc->Set(ControlMode::Position, ((-1 * get_position()) - 1000));
				intake_talon_left->Set(ControlMode::PercentOutput, 0);
				intake_talon_right->Set(ControlMode::PercentOutput, 0);
			}
			else
			{
				std::string gameData;
				gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
				if(gameData.length() > 0)
				{
					if (gameData[0] == 'L')
					{
						mode = 2;				
					}
					if (gameData[0] == 'R')
					{
						mode = 3;
					}		
				}
			}
		}
		if (mode == 2)
		{
			std::cout<<"Mode 2"<<std::endl;
			drive_talon_left_enc->SetSelectedSensorPosition(0, 0, 10);
			drive_talon_right_enc->SetSelectedSensorPosition(0, 0, 10);

			mode = 4;
		}
		if (mode == 3)
		{
			std::cout<<"Mode 3"<<std::endl;
			drive_talon_left_enc->SetSelectedSensorPosition(0, 0, 10);
			drive_talon_right_enc->SetSelectedSensorPosition(0, 0, 10);
			intake_talon_left->Set(ControlMode::PercentOutput, 0);
			intake_talon_right->Set(ControlMode::PercentOutput, 0);
			mode = 5;
		}
		if (mode == 4)
		{
			std::cout<<"Mode 4"<<std::endl;
			sync();
			if (drive_talon_right_enc->GetSelectedSensorPosition(0) < 35000 || drive_talon_left_enc->GetSelectedSensorPosition(0) > -35000)
			{
				drive_talon_right_enc->Set(ControlMode::Position, (get_position() + 1000));
				drive_talon_left_enc->Set(ControlMode::Position, ((-1 * get_position()) - 1000));
			}
    		else{
    			mode = 6;
    		}
		}
		if (mode == 5)
		{
			std::cout<<"Mode 5"<<std::endl;
			drive_talon_left_enc->Set(ControlMode::Position, -900);
    		drive_talon_right_enc->Set(ControlMode::Position, -900);
    		intake_talon_left->Set(ControlMode::PercentOutput, 0);
			intake_talon_right->Set(ControlMode::PercentOutput, 0);
    		if (abs(drive_talon_right_enc->GetSelectedSensorPosition(0) + 11000) < 200 || abs(drive_talon_left_enc->GetSelectedSensorPosition(0) + 11000) < 200)
    		{
    			mode = 7;
    		}
    		else{
    			drive_talon_left_enc->Set(ControlMode::Position, -11000);
    			drive_talon_right_enc->Set(ControlMode::Position, -11000);
    			if (abs(drive_talon_right_enc->GetSelectedSensorPosition(0) + 11000) < 200 || abs(drive_talon_left_enc->GetSelectedSensorPosition(0) + 11000) < 200)
    			{
    			mode = 7;
    			}
    		}
		}
		if (mode == 6)
		{
			std::cout<<"Mode 6"<<std::endl;
			drive_talon_left_enc->SetSelectedSensorPosition(0, 0, 10);
			drive_talon_right_enc->SetSelectedSensorPosition(0, 0, 10);
			mode = 8;
		}
		if (mode == 7)
		{
			std::cout<<"Mode 7"<<std::endl;
			drive_talon_left_enc->SetSelectedSensorPosition(0, 0, 10);
			drive_talon_right_enc->SetSelectedSensorPosition(0, 0, 10);
			intake_talon_left->Set(ControlMode::PercentOutput, 0);
			intake_talon_right->Set(ControlMode::PercentOutput, 0);
			mode = 9;
		}
 		if (mode == 8){
 			std::cout<<"Mode 8"<<std::endl;
 			drive_talon_left_enc->Set(ControlMode::Position, -10000);
    		drive_talon_right_enc->Set(ControlMode::Position, -10000);
    		if (abs(drive_talon_right_enc->GetSelectedSensorPosition(0) + 10000) < 200 || abs(drive_talon_left_enc->GetSelectedSensorPosition(0) + 10000) < 200)
    		{
    			mode = 10;
    		}
    		
 		}
 		if (mode == 9)
 		{
 			std::cout<<"Mode 9"<<std::endl;
 			intake_talon_left->Set(ControlMode::PercentOutput, 0);
			intake_talon_right->Set(ControlMode::PercentOutput, 0);
 			sync();
			if (drive_talon_right_enc->GetSelectedSensorPosition(0) < 83000 || drive_talon_left_enc->GetSelectedSensorPosition(0) > -83000)
			{
				drive_talon_right_enc->Set(ControlMode::Position, (get_position() + 1000));
				drive_talon_left_enc->Set(ControlMode::Position, ((-1 * get_position()) - 1000));
			}
    		else{
    			mode = 11;
    		}
 		}
 		if (mode == 10)
 		{
 			std::cout<<"Mode 10"<<std::endl;
 			intake_talon_left->Set(ControlMode::PercentOutput, -0.7);
			intake_talon_right->Set(ControlMode::PercentOutput, -0.7);	
 		}
 		if (mode == 11)
 		{
 			std::cout<<"Mode 11"<<std::endl;
 			drive_talon_left_enc->SetSelectedSensorPosition(0, 0, 10);
			drive_talon_right_enc->SetSelectedSensorPosition(0, 0, 10);
			intake_talon_left->Set(ControlMode::PercentOutput, 0);
			intake_talon_right->Set(ControlMode::PercentOutput, 0);
			mode = 13;
 		}
 		if (mode == 13)
 		{
 			std::cout<<"Mode 13"<<std::endl;
			drive_talon_left_enc->Set(ControlMode::Position, 18000);
    		drive_talon_right_enc->Set(ControlMode::Position, 18000);
    		intake_talon_left->Set(ControlMode::PercentOutput, 0);
			intake_talon_right->Set(ControlMode::PercentOutput, 0);
    		if (abs(drive_talon_right_enc->GetSelectedSensorPosition(0) -18000) < 200 || abs(drive_talon_left_enc->GetSelectedSensorPosition(0) - 18000) < 200)
    		{
    			mode = 15;
    		}
    		else{
    			drive_talon_left_enc->Set(ControlMode::Position, 18000);
    			drive_talon_right_enc->Set(ControlMode::Position, 18000);
    			if (abs(drive_talon_right_enc->GetSelectedSensorPosition(0) - 18000) < 200 || abs(drive_talon_left_enc->GetSelectedSensorPosition(0) - 18000) < 200)
    			{
    			mode = 15;
    			}
    		}
 		}
 		if (mode == 15)
 		{
 			std::cout<<"Mode 15"<<std::endl;
 			drive_talon_left_enc->SetSelectedSensorPosition(0, 0, 10);
			drive_talon_right_enc->SetSelectedSensorPosition(0, 0, 10);
			intake_talon_left->Set(ControlMode::PercentOutput, 0);
			intake_talon_right->Set(ControlMode::PercentOutput, 0);
			mode = 17;
 		}
 		if (mode == 17)
 		{
 			std::cout<<"Mode 17"<<std::endl;
 			
 			sync();
			if (drive_talon_right_enc->GetSelectedSensorPosition(0) < 40000 || drive_talon_left_enc->GetSelectedSensorPosition(0) > -40000)
			{
				drive_talon_right_enc->Set(ControlMode::Position, (get_position() + 1000));
				drive_talon_left_enc->Set(ControlMode::Position, ((-1 * get_position()) - 1000));
				intake_talon_left->Set(ControlMode::PercentOutput, 0);
				intake_talon_right->Set(ControlMode::PercentOutput, 0);
			}
    		else{
    			mode = 19;
    		}
 		}
 		if (mode == 19)
 		{
 			std::cout<<"Mode 19"<<std::endl;
 			intake_talon_left->Set(ControlMode::PercentOutput, 0);
			intake_talon_right->Set(ControlMode::PercentOutput, 0);
 			drive_talon_left_enc->SetSelectedSensorPosition(0, 0, 10);
			drive_talon_right_enc->SetSelectedSensorPosition(0, 0, 10);
			mode = 21;
 		}
 		if (mode == 21)
 		{
 			std::cout<<"Mode 21"<<std::endl;
 			drive_talon_left_enc->Set(ControlMode::Position, 12000);
    		drive_talon_right_enc->Set(ControlMode::Position, 12000);
    		intake_talon_left->Set(ControlMode::PercentOutput, 0);
			intake_talon_right->Set(ControlMode::PercentOutput, 0);
    		if (abs(drive_talon_right_enc->GetSelectedSensorPosition(0) -12000) < 200 || abs(drive_talon_left_enc->GetSelectedSensorPosition(0) - 12000) < 200)
    		{
    			mode = 23;
    		}
    		else{
    			drive_talon_left_enc->Set(ControlMode::Position, 12000);
    			drive_talon_right_enc->Set(ControlMode::Position, 12000);
    			if (abs(drive_talon_right_enc->GetSelectedSensorPosition(0) - 12000) < 200 || abs(drive_talon_left_enc->GetSelectedSensorPosition(0) - 12000) < 200)
    			{
    			mode = 23;
    			}
    		}

 		}
 		if (mode == 23)
 		{
 			std::cout<<"Mode 23"<<std::endl;
 			intake_talon_left->Set(ControlMode::PercentOutput, -0.7);
			intake_talon_right->Set(ControlMode::PercentOutput, -0.7);	
 		}
   		std::cout<<"Left Side: "<<drive_talon_left_enc->GetSelectedSensorPosition(0)<<std::endl;
		std::cout<<"Right Side: "<<drive_talon_right_enc->GetSelectedSensorPosition(0)<<std::endl;
		std::cout<<" "<<std::endl;	
	}

   	void TeleopInit() { 
	//Manual ScissorLift
		if(SmartDashboard::GetBoolean("DB/Button 1", 0)){
    		std::cout<<"Manual ScissorLift"<<std::endl;
    		scissorlift_talon_right->ConfigPeakOutputForward(1.0, 10);
    		scissorlift_talon_right->ConfigPeakOutputReverse(-1.0, 10);
    		scissorlift_talon_left->ConfigPeakOutputForward(1.0, 10);
    		scissorlift_talon_left->ConfigPeakOutputReverse(-1.0, 10);
		}
	//Regular Teleop
		else{
    		std::cout<<"Regular Teleop"<<std::endl;
    		scissorlift->start_loop(1);
		}
	}

    void TeleopPeriodic() {
	//Manual ScissorLift
		if(SmartDashboard::GetBoolean("DB/Button 1", 0)){
			scissorlift_talon_left->Set(ControlMode::PercentOutput, joy->GetRawAxis(1));
			scissorlift_talon_right->Set(ControlMode::PercentOutput, joy->GetRawAxis(5));
			std::cout<<scissorlift_talon_right->GetSelectedSensorPosition(0)<<":"<<scissorlift_talon_left->GetSelectedSensorPosition(0)<<std::endl;
		}
	//Regular Teleop
		else{
			joystick_scissor_lift->run_loop();
			intake->Start_Intake(1, -1);
			std::cout<<scissorlift_talon_right->GetSelectedSensorPosition(0)<<":"<<scissorlift_talon_left->GetSelectedSensorPosition(0)<<std::endl;
			drivebase->run_loop();
			
		}
	} 

	void DisabledInit() {
		scissorlift->stop_loop();
	}

    void DisabledPeriodic() {}
};
START_ROBOT_CLASS(Robot);
