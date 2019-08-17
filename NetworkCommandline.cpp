/*
* Network.c
 *
 *  Created on: 14.05.2017
 *      Author: cookie
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

// show status Line every [ms], 0 = off
unsigned long g_ulShowSerialStatsLine;

void NetworkCommandline_Init_Main();
void Console_Menu_Item_Help( void);
void Console_Menu_ShowStatus( void);
void Console_Menu_ShowStatusLine( void);
void Console_Menu_configRead( void);
void Console_Menu_configWrite( void);
void Console_Menu_configShow( void);

void Console_MenuConfig_Item_Help( void);

static Commandline::tCmdlineMenu cmdlineMenuMain[] =
{
	{"?", 							Console_Menu_Item_Help},
	{"help",						Console_Menu_Item_Help},
	{"showStatus",					Console_Menu_ShowStatus},
	{"showStatusLine",				Console_Menu_ShowStatusLine},
	{"configRead",					Console_Menu_configRead},
	{"configWrite",					Console_Menu_configWrite},
	{"configShow",					Console_Menu_configShow},
};

static Commandline::tCmdlineMenu cmdlineMenuConfig[] =
{
	{"?", 							Console_MenuConfig_Item_Help},
	{"help",						Console_MenuConfig_Item_Help},
	{"configShow",					Console_Menu_configShow},

	{"exit",						NetworkCommandline_Init_Main},
};

void NetworkCommandline_Init_Main()
{
	cmd.CmdlineMenuAnzahl = sizeof(cmdlineMenuMain) / sizeof(Commandline::tCmdlineMenu);
	strcpy ((char *) (cmd.CmdlineMenuPrompt), "main>");
	cmd.CmdlineMenuItem = cmdlineMenuMain;

	g_ulShowSerialStatsLine = 0;
}

void NetworkCommandline_Init()
{
	Serial.println("Start Network Commandline");

	// start listening for clientTelnets
	serverTelnet.begin();

	NetworkCommandline_Init_Main();
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
			Console_ShowStatusLine();
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


void Console_Menu_Item_Help( void)
{
	cmd.Console_sendString( (char*)
		"****************************************************************\r\n"
		"* Available commands are:                                      *\r\n"
		"*   help                       - displays available commands   *\r\n"
		"*   showStatus                 - show status                   *\r\n"
		"*   showStatusLine <s>        - show status, update every s ms *\r\n"
		"*   stop                       - Stop Motors                   *\r\n"
		"*   setMotorStop               - Stop Motors                   *\r\n"
		"*   setMotorAzimuthSpeed <s>   - set Azimuth s Speed           *\r\n"
		"*   setMotorElevationSpeed <s> - set Elevation s Speed         *\r\n"
		"*   setEnablePID [A/E] <0/1>   - enable auto motor control     *\r\n"
		"*   startRefRun                - Start both Reference Runs     *\r\n"
		"*   startRefRunAzimuth         - Start Azimuth Reference Run   *\r\n"
		"*   startRefRunElevation       - Start Elevation Reference Run *\r\n"
		"*   configRead                 - Read Config from EEprom       *\r\n"
		"*   configWrite                - Write Config toEEprom         *\r\n"
		"*   configShow                 - Show Configuration            *\r\n"
		"*   configAzimute              - Configure Azimute             *\r\n"
		"*   configElevation            - configure Elevation           *\r\n"
		"****************************************************************\r\n");
}

void Console_Menu_ShowStatus( void)
{
	// Clear Line
	cmd.Console_sendByte (ASCII_CR);
	cmd.vt100EraseToLineEnd();

	// show Values

	// show command prompt
	cmd.cmdlineRepaint();
}

void Console_Menu_ShowStatusLine( void)
{
	g_ulShowSerialStatsLine = cmd.cmdlineGetArgInt(1);
}

void Console_ShowStatusLine()
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
}

void Console_Menu_SetMotorStop( void)
{
	g_rototData[R_DOWN].MotorActive = false;
	g_rototData[R_UP].MotorActive = false;
}

void Console_Menu_configRead( void)
{
	EepromConfiguratonRead();
}

void Console_Menu_configWrite( void)
{
	EepromConfiguratonWrite();
}

void Console_Menu_configShow( void)
{
	clientTelnet.print( "\tUse Dhcp: ");
	clientTelnet.print( g_eepromStore.UseDhcp);
	clientTelnet.println( "");

	clientTelnet.print( "\tEncoder Counter Scaler Up: ");
	clientTelnet.print( g_eepromStore.EncoderCounterScaler[0]);
	clientTelnet.println( "");

	clientTelnet.print( "\tEncoder Counter Scaler Down: ");
	clientTelnet.print( g_eepromStore.EncoderCounterScaler[1]);
	clientTelnet.println( "");
}


void Console_MenuConfig_Item_Help( void)
{
	cmd.Console_sendString( (char*)
		"****************************************************************\r\n"
		"* Available commands are:                                      *\r\n"
		"*   help                       - displays available commands   *\r\n"
		"*   configShow                 - Show Config                   *\r\n"
		"*   exit                       - return to main menu           *\r\n"
		"****************************************************************\r\n");
}
