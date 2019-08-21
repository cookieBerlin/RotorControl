/*
 * Gpio.cpp
 */

#include "Global.h"

// ----------------------------------------------------------------------------
// - 	SetupGPIO
// ----------------------------------------------------------------------------
void SetupGPIO()
{
	pinMode(LED_BUILTIN, OUTPUT);		// initialize digital pin LED_BUILTIN as an output.
	digitalWrite(LED_BUILTIN, LOW);		// turn the LED off by making the voltage LOW

	pinMode(PIN_GPIO_KEY, INPUT);
	pinMode(PIN_GPIO_ROTOR_UP, INPUT);
	pinMode(PIN_GPIO_ROTOR_LED, OUTPUT);
	pinMode(PIN_GPIO_ROTOR_DOWN, INPUT);
	pinMode(PIN_GPIO_CONTROL_CW, INPUT);
	pinMode(PIN_GPIO_CONTROL_LED, OUTPUT);
	pinMode(PIN_GPIO_CONTROL_CCW, INPUT);
}

void GpioReadInput()
{
	g_gpio.Key = !digitalRead(PIN_GPIO_KEY);
	g_gpio.RotorUp = !digitalRead(PIN_GPIO_ROTOR_UP);
	g_gpio.RotorDown = !digitalRead(PIN_GPIO_ROTOR_DOWN);
	g_gpio.ControlCw = !digitalRead(PIN_GPIO_CONTROL_CW);
	g_gpio.ControlCcw = !digitalRead(PIN_GPIO_CONTROL_CCW);


	g_gpio.RotorLed = false;
	g_gpio.ControlLed = false;
	if(g_gpio.Key)
	{
		g_gpio.RotorLed = true;
		g_rototData[R_UP].MotorActive = false;
		g_rototData[R_DOWN].MotorActive = false;

		if(g_gpio.RotorUp || g_gpio.RotorDown)
		{
			int id = g_gpio.RotorUp ? R_UP : R_DOWN;

			g_rototData[id].MotorActive = false;
			if( g_gpio.ControlCw ^ g_gpio.ControlCcw)	// if one key pressed
			{
				g_gpio.ControlLed = true;
				g_rototData[id].MotorActive = true;
				g_rototData[id].MotorTurningCcw = g_gpio.ControlCcw;
			}
		}
		else
		{
			if( g_gpio.ControlCcw)
			{
				g_rototData[R_DOWN].CounterReferenced = true;
				g_rototData[R_DOWN].TargetPositionInDegree = NAN;
				EncoderReset(R_DOWN);
			}
			if( g_gpio.ControlCw)
			{
				g_rototData[R_UP].CounterReferenced = true;
				g_rototData[R_UP].TargetPositionInDegree = NAN;
				EncoderReset(R_UP);
			}
		}
	}
}

void GpioWriteOutput()
{
	digitalWrite(PIN_GPIO_ROTOR_LED, g_gpio.RotorLed);
	digitalWrite(PIN_GPIO_CONTROL_LED, g_gpio.ControlLed);
}
