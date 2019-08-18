/*
 * MotorControl.cpp
 */

#include "Global.h"
#include "DualVNH5019MotorShield.h"

// ----------------------------------------------------------------------------
// - 	Globals
// ----------------------------------------------------------------------------

#define OFF_VALUE	0
#define ON_VALUE	400

DualVNH5019MotorShield md( PIN_MD_INA1, PIN_MD_INB1, PIN_MD_PWM1, PIN_MD_EN1DIAG1, PIN_MD_CS1,
						   PIN_MD_INA2, PIN_MD_INB2, PIN_MD_PWM2, PIN_MD_EN2DIAG2, PIN_MD_CS2);

// ----------------------------------------------------------------------------
// - 	SetupMotorControl
// ----------------------------------------------------------------------------
void SetupMotorControl()
{
	//configure motor Control
	md.init();

	// stop all motor
	for( int i=0; i<NumberOfRotors; i++)
	{
		g_rototData[i].MotorActive = false;
	}

	// set actual config to GPIO output
	MotorControlUpdate();

	g_enable_PID = false;
}

// ----------------------------------------------------------------------------
// - 	MotorControlUpdate
// ----------------------------------------------------------------------------
void MotorControlUpdate()
{
	if( !g_gpio.Key && g_enable_PID)
	{
		for(int i=0; i<NumberOfRotors ; i++)
		{
			Serial.print("MotorControlUpdate PID");
			Serial.print(i);
			Serial.print(":");
			Serial.print(" Count: ");
			Serial.print(g_rototData[i].CounterInDegree );
			Serial.print(" Target: ");
			Serial.print(g_rototData[i].TargetPositionInDegree);

			g_rototData[i].MotorActive = g_rototData[i].TargetPositionInDegree != g_rototData[i].CounterInDegree;



			// TODO user counter limits
			g_rototData[i].MotorTurningCcw = g_rototData[i].TargetPositionInDegree > g_rototData[i].CounterInDegree;



			Serial.print(" MotorActive: ");
			Serial.print(g_rototData[i].MotorActive);
			Serial.print(" MotorTurningLeft: ");
			Serial.print(g_rototData[i].MotorTurningCcw);
			Serial.println();
		}

	}

	if( !g_rototData[R_DOWN].MotorActive)
	{
		md.setM1Speed(OFF_VALUE);
	}
	else if( g_rototData[R_DOWN].MotorTurningCcw)
	{
		md.setM1Speed(ON_VALUE);
	}
	else
	{
		md.setM1Speed(-ON_VALUE);
	}

	if( !g_rototData[R_UP].MotorActive)
	{
		md.setM2Speed(OFF_VALUE);
	}
	else if( g_rototData[R_UP].MotorTurningCcw)
	{
		md.setM2Speed(ON_VALUE);
	}
	else
	{
		md.setM2Speed(-ON_VALUE);
	}
}

// ----------------------------------------------------------------------------
// - 	MotorContolGetCurrentMilliamps
// ----------------------------------------------------------------------------
int MotorContolGetCurrentMilliamps(int motor)
{
	return (motor==0) ? md.getM1CurrentMilliamps() : md.getM2CurrentMilliamps();
}

// ----------------------------------------------------------------------------
// - 	MotorContolGetFault
// ----------------------------------------------------------------------------
bool MotorContolGetFault(int motor)
{
	return (motor==0) ? md.getM1Fault() : md.getM2Fault();
}
