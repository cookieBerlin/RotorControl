
#define EXTERN
#include "Global.h"

// ----------------------------------------------------------------------------
// - 	SetupSerial
// ----------------------------------------------------------------------------
void SetupSerial()
{
	// Open serial communications and wait for port to open:
	Serial.begin(9600);

	// this check is only needed on the Leonardo: wait for serial port to connect. Needed for native USB port only
	//while (!Serial);

	Serial1.begin(115200);
	Serial1.println("Hello World");
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
	DisplayTextLine(1, "V0.0.5    @DB3AC");

	EepromConfiguratonRead();

	SetupMotorControl();

	SetupNetwork();
	g_networkRotcrl[R_UP] =   new NetworkROTCRL(4533, &g_rototData[R_UP].CounterInDegree,   NULL, &g_rototData[R_UP].TargetPositionInDegree,   NULL);
	g_networkRotcrl[R_DOWN] = new NetworkROTCRL(4534, &g_rototData[R_DOWN].CounterInDegree, NULL, &g_rototData[R_DOWN].TargetPositionInDegree, NULL);
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
	GpioReadInput();
	EncoderUpdate();
	MotorControlUpdate();
	DisplayShowStatus();
	GpioWriteOutput();
}


// ----------------------------------------------------------------------------
// - 	loop - The loop function is called in an endless loop
// ----------------------------------------------------------------------------
void loop()
{
	g_networkRotcrl[R_UP]->HandleCommunication();
	g_networkRotcrl[R_DOWN]->HandleCommunication();
	NetworkCommandline_handleCommunication();

	CentralContol();
}
