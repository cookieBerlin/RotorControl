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
}

void GpioWriteOutput()
{
	g_gpio.RotorLed = g_gpio.ControlCw;
	g_gpio.ControlLed = g_gpio.ControlCcw;

	digitalWrite(PIN_GPIO_ROTOR_LED, g_gpio.RotorLed);
	digitalWrite(PIN_GPIO_CONTROL_LED, g_gpio.ControlLed);
}
