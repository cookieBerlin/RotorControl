
#define EXTERN
#include "Global.h"

// ----------------------------------------------------------------------------
// - 	SetupGPIO
// ----------------------------------------------------------------------------
void SetupGPIO()
{

	pinMode(LED_BUILTIN, OUTPUT);		// initialize digital pin LED_BUILTIN as an output.
	digitalWrite(LED_BUILTIN, LOW);		// turn the LED off by making the voltage LOW
}

// ----------------------------------------------------------------------------
// - 	SetupSerial
// ----------------------------------------------------------------------------
void SetupSerial()
{
	// Open serial communications and wait for port to open:
	Serial.begin(9600);

	// this check is only needed on the Leonardo: wait for serial port to connect. Needed for native USB port only
	//while (!Serial);
}

// ----------------------------------------------------------------------------
// - 	setup - The setup function is called once at startup of the sketch
// ----------------------------------------------------------------------------
void setup()
{
	SetupGPIO();
	SetupSerial();

	SetupDisplay();

	// show start message
	DisplayTextLine(0, "FTM25/6 Controller");
	DisplayTextLine(1, "V0.0.3    @DB3AC");

	SetupEepromConfiguraton();

	SetupMotorControl();

	SetupNetwork();

	SetupEncoder();


	delay(1500);
	DisplayClear();
}


// ----------------------------------------------------------------------------
// - 	loop - The loop function is called in an endless loop
// ----------------------------------------------------------------------------
void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);	// turn the LED on by making the voltage High

	EncoderUpdate();
	MotorControlUpdate();
	DisplayShowStatus();

	digitalWrite(LED_BUILTIN, LOW);		// turn the LED off by making the voltage LOW

	delay(100);
}
