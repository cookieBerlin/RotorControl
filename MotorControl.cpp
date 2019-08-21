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
		g_rototData[i].TargetPositionInDegree = NAN;
	}

	// set actual config to GPIO output
	MotorControlUpdate();
}

// ----------------------------------------------------------------------------
// - 	MotorControlUpdate
// ----------------------------------------------------------------------------
void MotorControlUpdate()
{
	if( !g_gpio.Key)
	{
		for(int i=0; i<NumberOfRotors ; i++)
		{
			if( (g_rototData[i].TargetPositionInDegree == NAN))
			{
				g_rototData[i].MotorActive = false;

				Serial.print(" MotorActive: NAN");
			}
			else
			{
				g_rototData[i].MotorActive = abs(g_rototData[i].TargetPositionInDegree - g_rototData[i].CounterInDegree) > 1.0;

				// TODO user counter limits
				g_rototData[i].MotorTurningCcw = g_rototData[i].TargetPositionInDegree < g_rototData[i].CounterInDegree;
			}
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

	 g_rototData[R_DOWN].MotorCurrentInMa = md.getM1CurrentMilliamps();
	 g_rototData[R_UP].MotorCurrentInMa = md.getM2CurrentMilliamps();
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
