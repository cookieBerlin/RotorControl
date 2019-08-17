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

//	Serial.print(" Key: ");
//	Serial.print(g_gpio.Key);
//
//	Serial.print(", RotorUp: ");
//	Serial.print(g_gpio.RotorUp);
//
//	Serial.print(", RotorLed: #");
//	Serial.print(g_gpio.RotorLed);
//
//	Serial.print(", RotorDown: ");
//	Serial.print(g_gpio.RotorDown);
//
//	Serial.print(", ControlCw: ");
//	Serial.print(g_gpio.ControlCw);
//
//	Serial.print(", ControlLed: #");
//	Serial.print(g_gpio.ControlLed);
//
//	Serial.print(", ControlCcw: ");
//	Serial.print(g_gpio.ControlCcw);
//
//	Serial.println();

	g_gpio.RotorLed = false;
	g_gpio.ControlLed = false;
	if(g_gpio.Key)
	{
		g_gpio.RotorLed = true;
		if(g_gpio.RotorUp || g_gpio.RotorDown)
		{
			int id = g_gpio.RotorUp ? R_UP : R_DOWN;

			g_rototData[id].MotorActive = false;
			if( g_gpio.ControlCw ^ g_gpio.ControlCcw)	// if one key pressed
			{
				g_gpio.ControlLed = true;
				g_rototData[id].MotorActive = true;
				g_rototData[id].MotorTurningLeft = g_gpio.ControlCw;
			}
		}
		else
		{
			if( g_gpio.ControlCcw)
			{
				g_rototData[R_DOWN].CounterReferenced = true;
				EncoderReset(R_DOWN);
			}
			if( g_gpio.ControlCw)
			{
				g_rototData[R_UP].CounterReferenced = true;
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
