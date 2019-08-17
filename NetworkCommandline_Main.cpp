/*
* Network.c
 */

#include "Global.h"
#include "Arduino.h"
#include <SPI.h>
#include <Ethernet2.h>
#include "libraries/commandline/commandline.h"
#include "libraries/commandline/vt100.h"

extern Commandline cmd;
extern EthernetClient clientTelnet;


void Console_Menu_ConfigRead( void);
void Console_Menu_ConfigWrite( void);
void Console_Menu_ConfigShow( void);
void Console_Menu_SetMotorStop( void);
void Console_Menu_Help( void);
void Console_Menu_ShowGpio( void);
void Console_Menu_ShowStatus( void);
void Console_Menu_ShowStatusLine( void);

static Commandline::tCmdlineMenu cmdlineMenuMain[] =
{
	{"?", 							Console_Menu_Help},
	{"configRead",					Console_Menu_ConfigRead},
	{"configWrite",					Console_Menu_ConfigWrite},
	{"configShow",					Console_Menu_ConfigShow},
	{"help",						Console_Menu_Help},
	{"setMotorStop",				Console_Menu_SetMotorStop},
	{"showGpio",					Console_Menu_ShowGpio},
	{"showStatus",					Console_Menu_ShowStatus},
	{"showStatusLine",				Console_Menu_ShowStatusLine},
};

void NetworkCommandline_Main_Activate()
{
	cmd.CmdlineMenuAnzahl = sizeof(cmdlineMenuMain) / sizeof(Commandline::tCmdlineMenu);
	strcpy ((char *) (cmd.CmdlineMenuPrompt), "main>");
	cmd.CmdlineMenuItem = cmdlineMenuMain;
}

//##################################################################################################################

void Console_Menu_ConfigRead( void)
{
	EepromConfiguratonRead();
}

void Console_Menu_ConfigWrite( void)
{
	EepromConfiguratonWrite();
}

void Console_Menu_ConfigShow( void)
{
	clientTelnet.print( "\tUse Dhcp:                         ");
	clientTelnet.print( g_eepromStore.UseDhcp);
	clientTelnet.println( "");

	clientTelnet.print( "\tEncoder Counter Scaler Up / Down: ");
	clientTelnet.print( g_eepromStore.EncoderCounterScaler[R_UP]);
	clientTelnet.print( " / ");
	clientTelnet.print( g_eepromStore.EncoderCounterScaler[R_DOWN]);
	clientTelnet.println( "");
}

void Console_Menu_Help( void)
{
	cmd.Console_sendString( (char*)
		"****************************************************************\r\n"
		"* Available commands are:                                      *\r\n"
		"*   configRead                 - Read Config from EEprom       *\r\n"
		"*   configWrite                - Write Config toEEprom         *\r\n"
		"*   configShow                 - Show Configuration            *\r\n"
		"*   help                       - displays available commands   *\r\n"
		"*   setMotorStop               - Stop Motors                   *\r\n"
		"*   showGpio                   - show GPIO values              *\r\n"
		"*   showStatus                 - show status                   *\r\n"
		"*   showStatusLine <s>         - update status every s ms      *\r\n"
		"****************************************************************\r\n");
}

void Console_Menu_ShowGpio( void)
{
	clientTelnet.print(" Key: ");
	clientTelnet.print(g_gpio.Key);

	clientTelnet.print(", RotorUp: ");
	clientTelnet.print(g_gpio.RotorUp);

	clientTelnet.print(", RotorLed: #");
	clientTelnet.print(g_gpio.RotorLed);

	clientTelnet.print(", RotorDown: ");
	clientTelnet.print(g_gpio.RotorDown);

	clientTelnet.print(", ControlCw: ");
	clientTelnet.print(g_gpio.ControlCw);

	clientTelnet.print(", ControlLed: ");
	clientTelnet.print(g_gpio.ControlLed);

	clientTelnet.print(", ControlCcw: ");
	clientTelnet.print(g_gpio.ControlCcw);

	clientTelnet.println();
}

void Console_Menu_ShowStatus( void)
{
	// Clear Line
	cmd.Console_sendByte (ASCII_CR);
	cmd.vt100EraseToLineEnd();

	// show Values
	clientTelnet.print( "Rotor UP/DOWN:");

	clientTelnet.print( "\tCounter: ");
	clientTelnet.print( g_rototData[R_DOWN].CounterInDegree);
	clientTelnet.print( "/");
	clientTelnet.print( g_rototData[R_UP].CounterInDegree);
	clientTelnet.print(" Degree");

	clientTelnet.print( "\tCounter: ");
	clientTelnet.print( g_rototData[R_DOWN].CounterInSteps);
	clientTelnet.print( "/");
	clientTelnet.print( g_rototData[R_UP].CounterInSteps);
	clientTelnet.print(" Steps");

	clientTelnet.print( "\tReferenced: ");
	clientTelnet.print( g_rototData[R_DOWN].CounterReferenced);
	clientTelnet.print( "/");
	clientTelnet.print( g_rototData[R_UP].CounterReferenced);
	clientTelnet.print(" Steps");

	clientTelnet.print( "\tMotorActive: ");
	clientTelnet.print( g_rototData[R_DOWN].MotorActive);
	clientTelnet.print( "/");
	clientTelnet.print( g_rototData[R_UP].MotorActive);
	clientTelnet.print(" Steps");

	clientTelnet.print( "\tMotorTurningLeft: ");
	clientTelnet.print( g_rototData[R_DOWN].MotorTurningLeft);
	clientTelnet.print( "/");
	clientTelnet.print( g_rototData[R_UP].MotorTurningLeft);
	clientTelnet.print(" Steps");

	clientTelnet.print( "\tTargetPosition: ");
	clientTelnet.print( g_rototData[R_DOWN].TargetPositionInDegree);
	clientTelnet.print( "/");
	clientTelnet.print( g_rototData[R_UP].TargetPositionInDegree);
	clientTelnet.print(" Degree");

	clientTelnet.println();
}

void Console_Menu_ShowStatusLine( void)
{
	g_ulShowSerialStatsLine = cmd.cmdlineGetArgInt(1);
}

void Console_Menu_SetMotorStop( void)
{
	g_rototData[R_DOWN].MotorActive = false;
	g_rototData[R_UP].MotorActive = false;
}