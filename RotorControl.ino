
#define EXTERN
#include "Global.h"


// ----------------------------------------------------------------------------
// - 	setup - The setup function is called once at startup of the sketch
// ----------------------------------------------------------------------------
void setup()
{

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
}
