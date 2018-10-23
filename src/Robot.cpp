/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

//Our Includes
#include "controller_helper.h"
#include "driver_helper.hpp"
#include "motor_helper.hpp"

//other includes
#include "Robot.h"

//cpp includes
#include <iostream>

//FIRST Includes
#include <SmartDashboard/SmartDashboard.h>
#include <Timer.h>
#include <Solenoid.h>
#include <Compressor.h>

class PracticeRobot : public frc::SampleRobot
{
	public:
	PracticeRobot()
	{
		// Note SmartDashboard is not initialized here, wait until
		// RobotInit to make SmartDashboard calls
		//m_robotDrive.SetExpiration(0.1);
	}

	void RobotInit()
	{
		comp->SetClosedLoopControl(true);
	}

	void Autonomous()
	{

	}

	void OperatorControl()
	{
		bool controlSystem = true; //true is driver station and false is for the controllers
		controlSystem = LeftButtonHub.GetRawButton(Generic_Controller_Left::SWITCH_ARCADE_RIGHT);

		/////////////1 for driver station and -1 for xbox
		if (controlSystem)
		{
			//Commands for the driver station
			m_driveSystem->moveCtrl = RightDriveJoystick.GetYChannel();
			m_driveSystem->rotateCtrl = LeftDriveJoystick.GetXChannel();
			m_driveSystem->reverseDrive = 1;
		}
		else
		{
			//Commands for the xbox controller
			m_driveSystem->moveCtrl = Axis_XBOX::XBOX_RIGHT_JOYSTICK_Y;
			m_driveSystem->rotateCtrl = Axis_XBOX::XBOX_LEFT_JOYSTICK_X;
			m_driveSystem->reverseDrive = -1;
		}

		//m_robotDrive.SetSafetyEnabled(true);
		//dont know
		while (IsOperatorControl() && IsEnabled())
		{
			bool halfSpeed = false;
			halfSpeed = LeftButtonHub.GetRawButton(Generic_Controller_Left::SWITCH_A);

			if (halfSpeed)
			{
				m_driveSystem->motorMultiplier = 0.5;
				SmartDashboard::PutString("DB/String 0", "Motors at HALF speed");
			}
			else
			{
				m_driveSystem->motorMultiplier = 1.0;
				SmartDashboard::PutString("DB/String 0", "Motors at FULL speed");
			}

			//Drive system
			m_driveSystem->arcadeDrive();

			//Checks the Fire and Release buttons to set the solenoid state
			if (RightButtonHub.GetRawButton(Generic_Controller_Right::BUTTON_FIRE))
			{
				solenoidState = true;
			}
			else if (RightButtonHub.GetRawButton(Generic_Controller_Right::BUTTON_RELEASE))
			{
				solenoidState = false;
			}

			//Checks the solenoidState variable and then sets the solenoid accordingly
			if (solenoidState == true)
			{
				scoopSolenoid.Set(true);
			}
			else if (solenoidState == false)
			{
				scoopSolenoid.Set(false);
			}





			//frc::Wait(0.005);
		}
	}

	void Test() {}

	private:
		enum Channel_Controller
		{
			//The number correlates to the 'USB Order' number on the driver station
			XBOX_CONTROLLER = 0,				//When we are using the logitech and xbox controllers
			//The logitech controller and the trigger joystick share the same channel because the FRC driver station only allows for 6 (0-5) channels
			LOGITECH_CONTROLLER = 1,			//When we are using the logitech and xbox controllers
			FLIGHT_JOYSTICK = 1,				//When we are using the driver station
			LEFT_DRIVE_JOYSTICK = 2,			//When we are using the driver station
			LEFT_BUTTON_HUB = 3,				//When we are using the driver station
			RIGHT_DRIVE_JOYSTICK = 4,			//When we are using the driver station
			RIGHT_BUTTON_HUB = 5				//When we are using the driver station
		};

		enum Channel_PWM
		{
			//Channels for the PWM motors on the roboRIO
			LEFT_DRIVE = 0,
			RIGHT_DRIVE = 1,
		};

		enum Channel_Solenoid
		{
			SCOOP_SOLENOID = 0
		};


		frc::Joystick XboxController { Channel_Controller::XBOX_CONTROLLER };
		frc::Joystick LogitechController { Channel_Controller::LOGITECH_CONTROLLER };
		frc::Joystick FlightJoystick { Channel_Controller::FLIGHT_JOYSTICK };
		frc::Joystick LeftDriveJoystick { Channel_Controller::LEFT_DRIVE_JOYSTICK };
		frc::Joystick LeftButtonHub { Channel_Controller::LEFT_BUTTON_HUB };
		frc::Joystick RightDriveJoystick { Channel_Controller::RIGHT_DRIVE_JOYSTICK };
		frc::Joystick RightButtonHub { Channel_Controller::RIGHT_BUTTON_HUB };

		frc::Spark* m_leftDrive = new frc::Spark(Channel_PWM::LEFT_DRIVE);
		frc::Spark* m_rightDrive = new frc::Spark(Channel_PWM::RIGHT_DRIVE);

		//Our BjorgDrive systems for driving the robot and for using the winch, the function takes in two motors and two joysticks from above
		//&RightDriveJoystick and &LeftDriveJoystick for Driver Station
		//&XboxController and &XboxController for XBOX Controller
		BjorgDrive* m_driveSystem = new BjorgDrive(m_leftDrive, m_rightDrive, &RightDriveJoystick, &LeftDriveJoystick);

		Compressor *comp = new Compressor(compChannel);

		frc::Solenoid scoopSolenoid {Channel_Solenoid::SCOOP_SOLENOID};



		static const int compChannel = 0;

		bool solenoidState = false;

};
START_ROBOT_CLASS(Robot)
