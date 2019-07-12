
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

	EepromConfiguratonRead();

	SetupMotorControl();

	SetupNetwork();
	NetworkROTCRL_Init();
	NetworkCommandline_Init();

	SetupEncoder();


	delay(1500);
	DisplayClear();
}

// ----------------------------------------------------------------------------
// - 	CentralContol
// ----------------------------------------------------------------------------
void CentralContol()
{
	static unsigned	long previousMillis = 0;        // will store last time
	unsigned 		long currentMillis = millis();

	// check if next call
	if ( (currentMillis - previousMillis) <= CONTROL_INTERVALL)
	{
		digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
		return;
	}
	previousMillis = currentMillis;	// save the current time

	digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)


	// Control commands
	EncoderUpdate();
	MotorControlUpdate();
	DisplayShowStatus();
}


// ----------------------------------------------------------------------------
// - 	loop - The loop function is called in an endless loop
// ----------------------------------------------------------------------------
void loop()
{
	NetworkROTCRL_handleCommunication();
	NetworkCommandline_handleCommunication();

	CentralContol();
}
