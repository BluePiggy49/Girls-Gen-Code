#include <crossthelineauto.h>

void CrossTheLine::Start_Autonomous(){
	if (Start)
	{
		timer.Reset();
		timer.Start();
		Start = false;
	}
	//scissorlift->start_loop(1);
	Run_Autonomous();
}

void CrossTheLine::Run_Autonomous(){
	//bool FMS = frc::DriverStation::GetInstance().IsFMSAttached();
	//if(FMS){
	/*std::string gameData;
	gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		if(gameData.length() > 0)
		{
			if (gameData[0] == 'L')
			{*/
				mode = 1;
				int pig = 1;
				if (pig == 1)
				{
					Us_Left();
				}
				
			/*}
			else
			{
				Them_Left();	
			}
		}
	//}*/
}
void CrossTheLine::Us_Left(){
	if (mode == 1)
	{
		std::cout<<"Mode 1"<<std::endl;
		drive_talon_left_enc->Set(ControlMode::Position, -60000);
   		drive_talon_right_enc->Set(ControlMode::Position, 60000);
   		std::cout<<"Checkpoint"<<std::endl;
   		int left_error = abs(drive_talon_left_enc->GetSelectedSensorPosition(0) + 60000);
   		int right_error = abs(drive_talon_right_enc->GetSelectedSensorPosition(0) - 60000);
   		if (left_error < 50 && right_error < 50)
   		{
   			mode = 2;
   		}
   		else{
   			drive_talon_left_enc->Set(ControlMode::Position, -60000);
   			drive_talon_right_enc->Set(ControlMode::Position, 60000);
   		}
   	}
   	if (mode == 2)
	{
		std::cout<<"Mode 2"<<std::endl;
		drive_talon_left_enc->Set(ControlMode::Position, -70000);
   		drive_talon_right_enc->Set(ControlMode::Position, 70000);
   		int left_error = abs(drive_talon_left_enc->GetSelectedSensorPosition(0) + 70000);
   		int right_error = abs(drive_talon_right_enc->GetSelectedSensorPosition(0) - 70000);
   		if (left_error < 50 && right_error < 50)
   		{
   			mode = 3;
   		}
   		
	}
	
	if (mode == 3)
	{
		std::cout<<"Mode 3"<<std::endl;
		drive_talon_left_enc->SetSelectedSensorPosition(0, 0, 10);
		drive_talon_right_enc->SetSelectedSensorPosition(0, 0, 10);
		mode = 4;
	}
	if (mode == 4)
	{
		std::cout<<"Mode 4"<<std::endl;
		drive_talon_left_enc->Set(ControlMode::Position, -10000);
    	drive_talon_right_enc->Set(ControlMode::Position, -10000);
	}
}
void CrossTheLine::Them_Left(){
	drive_talon_left_enc->Set(ControlMode::Position, -60000);
	drive_talon_right_enc->Set(ControlMode::Position, 60000);
}