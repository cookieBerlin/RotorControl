/*
* Network.c
 */

#include "Global.h"
#include "Arduino.h"
#include <SPI.h>
#include <Ethernet2.h>
#include "libraries/commandline/commandline.h"
#include "libraries/commandline/vt100.h"

// ----------------------------------------------------------------------------
// - 	Network Service
// ----------------------------------------------------------------------------
// Configure serverTelnet Port to 23, default of telnet
EthernetServer serverTelnet(23);

EthernetClient clientTelnet;

void NetworkConsole_sendString( char *BufferPtr);
Commandline cmd( NetworkConsole_sendString);

void Console_UpdateStatusLine();

void NetworkCommandline_Init()
{
	Serial.println("Start Network Commandline");

	// start listening for clientTelnets
	serverTelnet.begin();

	NetworkCommandline_Main_Activate();
}

void NetworkCommandline_handleCommunication()
{
	typedef enum tag_enum_NetWorkCommunication_StateMaschine
	{
		eWaitForNewClient,
		eWaitForData,
		eClientDisconnect,
	} enum_NetWorkCommunication_StateMaschine;

	static enum_NetWorkCommunication_StateMaschine eState = eWaitForNewClient;

	switch ( eState )
	{
		case eWaitForNewClient:
			// wait for a new client:
			clientTelnet = serverTelnet.available();
			if (clientTelnet)
			{
				Serial.println("Telnet Client Connected");
		    	eState = eWaitForData;
			}
			break;
		case eWaitForData:
			if( clientTelnet.connected() == 0 )
			{
		    	eState = eClientDisconnect;
			}
			else if (clientTelnet.available())
			{
				char c = clientTelnet.read();

				cmd.cmdlineDecodeInput(c);
			}
			Console_UpdateStatusLine();
			break;
		case eClientDisconnect:
		    Serial.println("Telnet Client Disconnected");
		    clientTelnet = 0;
		    eState = eWaitForNewClient;
			break;
	}
}

///				Send String to Console
void NetworkConsole_sendString( char *BufferPtr)
{
	clientTelnet.print(BufferPtr);
}

void Console_UpdateStatusLine()
{
	static unsigned	long previousMillis = 0;        // will store last time
	unsigned 		long currentMillis = millis();

	if ( (g_ulShowSerialStatsLine == 0) ||
		 ((currentMillis - previousMillis) <= g_ulShowSerialStatsLine) )
	{
		return;
	}
	previousMillis = currentMillis;	// save the current time

	Console_Menu_ShowStatus();

	// show command prompt
	cmd.cmdlineRepaint();
}
