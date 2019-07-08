
#define EXTERN
#include "Global.h"


//The setup function is called once at startup of the sketch
void setup()
{
	SetupEepromConfiguraton();
	SetupDisplay();
	SetupEncoder();
	SetupNetwork();
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
}
