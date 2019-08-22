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

int32_t	CounterReferenceRun[NumberOfRotors];

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

void MotorControlUpdate_StartReferenceRun(int i)
{
	g_rototData[i].MotorActive = true;
	g_rototData[i].MotorTurningCcw = true;

	CounterReferenceRun[i] = g_rototData[i].CounterInSteps;

	g_rototData[i].CounterReferenced = ReferenceSearing;
}

void MotorControlUpdate_ReferenceSearing(int i)
{
	int32_t diff = abs(CounterReferenceRun[i] - g_rototData[i].CounterInSteps);
	Serial.print("\tDiff: ");
	Serial.println(diff);

	if( diff < 5 )
	{
		// found end switch (no moving)
		g_rototData[i].CounterReferenced = StopReferenceRun;
		g_rototData[i].MotorActive = false;
	}

	CounterReferenceRun[i] = g_rototData[i].CounterInSteps;
}

void MotorControlUpdate_StopReferenceRun(int i)
{
	EncoderReset(i);
	g_rototData[i].CounterReferenced = Referenced;
}

void MotorControlUpdate_Referenced(int i)
{
	if( (g_rototData[i].TargetPositionInDegree == NAN))
	{
		g_rototData[i].MotorActive = false;
	}
	else
	{
		g_rototData[i].MotorActive = abs(g_rototData[i].TargetPositionInDegree - g_rototData[i].CounterInDegree) > 1.0;
		g_rototData[i].MotorTurningCcw = g_rototData[i].TargetPositionInDegree < g_rototData[i].CounterInDegree;

		double limit = (360 + g_eepromStore.EncoderCounterOffset[R_UP]);
		if(g_rototData[i].TargetPositionInDegree < limit)
		{
			g_rototData[i].MotorTurningCcw = !g_rototData[i].MotorTurningCcw;
		}
		if(g_rototData[i].CounterInDegree < limit)
		{
			g_rototData[i].MotorTurningCcw = !g_rototData[i].MotorTurningCcw;
		}
	}
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
			if( g_rototData[i].CounterReferenced == StartReferenceRun)
			{
				MotorControlUpdate_StartReferenceRun(i);
			}
			else if( g_rototData[i].CounterReferenced == ReferenceSearing)
			{
				MotorControlUpdate_ReferenceSearing(i);
			}
			else if( g_rototData[i].CounterReferenced == StopReferenceRun)
			{
				MotorControlUpdate_StopReferenceRun(i);
			}
			else if( g_rototData[i].CounterReferenced == Referenced)
			{
				MotorControlUpdate_Referenced(i);
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
