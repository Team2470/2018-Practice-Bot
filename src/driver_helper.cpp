//*****************************************************************************
// Filename:            driver_helper.cpp
//
// Revision Record:
//   Author             Date       Description
//   ------------------ ---------- --------------------------------------------
//   Chris Struck       Jan. 2018  Initial design.
//
// Description:
//    This class operates a SpeedController group using Differential Drive.
//    The class has the ability to drive using tele-operated Arcade Drive, 
//        autonomous arcade drive, and should be updated for other versions of 
//        as needed (tank, H, etc.).
// 
// Dependencies:
//    None
//*****************************************************************************

/******************************************************************************
 * Include Files
 *****************************************************************************/
// System Includes
#include <string>
#include <cmath>
#include <algorithm>

// FIRST Includes
#include <Drive/DifferentialDrive.h>
#include <Joystick.h>
#include <Spark.h>

// Our Includes
#include "driver_helper.hpp"

/*-----------------------------------------------------------------------------
 * FUNCTION NAME:    BjorgDrive
 *---------------------------------------------------------------------------*/
BjorgDrive::BjorgDrive(frc::Spark *m_leftMotor, frc::Spark *m_rightMotor, 
    frc::Joystick *controller1, frc::Joystick *controller2)
{
    m_robotDrive = new frc::DifferentialDrive { *m_leftMotor, *m_rightMotor };
    driveController1 = controller1;
    driveController2 = controller2;
    m_robotDrive->SetExpiration(0.1);
};



/*-----------------------------------------------------------------------------
 * FUNCTION NAME:    arcadeDrive
 *---------------------------------------------------------------------------*/
void BjorgDrive::arcadeDrive()
{
    m_robotDrive->SetSafetyEnabled(true);

    setMovement();
    setRotate(rotateEnable);
    /////////////////////////////////////////////////////////////////////////////////////////

    //joystick debounce
    if(movementValue <= 0.1f && movementValue >= -0.1f)
    {
        movementValue = 0.0f;
    }

    if(rotationValue <= 0.1f && rotationValue >= -0.1f)
    {
        rotationValue = 0.0f;
    }

    m_robotDrive->ArcadeDrive(reverseDrive * motorMultiplier * movementValue, rotateMult * rotationValue, sqrInputs);

}

/*-----------------------------------------------------------------------------
 * FUNCTION NAME:    arcadeDrive
 *---------------------------------------------------------------------------*/
void BjorgDrive::arcadeDrive(double movement, double rotate)
{
    m_robotDrive->SetSafetyEnabled(true);
    m_robotDrive->ArcadeDrive(movement, rotate);
}

/*-----------------------------------------------------------------------------
 * FUNCTION NAME:    twoBtnDrive
 *---------------------------------------------------------------------------*/
void BjorgDrive::twoBtnDrive()
{
    // Combines two inputs into one value
    movementValue = driveController1->GetRawAxis(fwdDrive) - driveController1->GetRawAxis(bckDrive);
}
	
/*-----------------------------------------------------------------------------
 * FUNCTION NAME:    twoBtnRotate
 *---------------------------------------------------------------------------*/
void BjorgDrive::twoBtnRotate()
{
    float triggerVal = 0.0;
    float maxVal = 0.0;
    float addVal = 0.0;

    //Combines the trigger values so if the right trigger is pressed the robot 
    //turns right and if the left trigger is pressed the robot turns left
    triggerVal = driveController1->GetRawAxis(lftTurn) -
        driveController1->GetRawAxis(rtTurn);
    //Finds the max value from the combined trigger value and right joystick 
    //x-axis
    maxVal = std::max(double(std::abs(triggerVal)), std::abs(driveController1->GetRawAxis(joystickInt)));
    //Adds the combined trigger value and the right joystick x-axis to find the
    //sign (positive or negative) of the max value
    addVal = triggerVal + driveController1->GetRawAxis(joystickInt);

    //Combines the max value and the add value to decide the rotate value so it
    //turns correctly
    if (addVal < 0)
    {
        rotationValue = -1 * maxVal;
    }
    else
    {
        rotationValue = maxVal;
    }
}
    
/*-----------------------------------------------------------------------------
 * FUNCTION NAME:    setMovement
 *---------------------------------------------------------------------------*/
void BjorgDrive::setMovement()
{
    if (multiMove)
    {
        twoBtnDrive();
    }

    else
    {
        movementValue = driveController1->GetRawAxis(moveCtrl);
    }
};

/*-----------------------------------------------------------------------------
 * FUNCTION NAME:    setRotate
 *---------------------------------------------------------------------------*/
void BjorgDrive::setRotate(bool rotateEn)
{
    if (multiRotate)
    {
        twoBtnRotate();
    }

    else
    {
        rotationValue = (rotateEn ? driveController2->GetRawAxis(rotateCtrl) : 0);
    }
};
