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


void Console_Menu_Config( void);
void Console_Menu_Main_Help( void);
void Console_Menu_ResetCounterDown( void);
void Console_Menu_ResetCounterUp( void);
void Console_Menu_SetEnablePID( void);
void Console_Menu_SetRotorDown( void);
void Console_Menu_SetRotorUp( void);
void Console_Menu_SetRotorStop( void);
void Console_Menu_ShowGpio( void);
void Console_Menu_ShowStatus( void);
void Console_Menu_ShowStatusLine( void);

static Commandline::tCmdlineMenu cmdlineMenuMain[] =
{
	{"?", 							Console_Menu_Main_Help},
	{"config",					    Console_Menu_Config},
	{"help",						Console_Menu_Main_Help},
	{"resetCounterDown",			Console_Menu_ResetCounterDown},
	{"resetCounterUp",				Console_Menu_ResetCounterUp},
	{"setEnablePID",				Console_Menu_SetEnablePID},
	{"setRotorDown",				Console_Menu_SetRotorDown},
	{"setRotorUp",					Console_Menu_SetRotorUp},
	{"setRotorStop",				Console_Menu_SetRotorStop},
	{"showGpio",					Console_Menu_ShowGpio},
	{"showStatus",					Console_Menu_ShowStatus},
	{"showStatusLine",				Console_Menu_ShowStatusLine},
	{"stop",						Console_Menu_SetRotorStop},
};

void NetworkCommandline_Main_Activate()
{
	cmd.CmdlineMenuAnzahl = sizeof(cmdlineMenuMain) / sizeof(Commandline::tCmdlineMenu);
	strcpy ((char *) (cmd.CmdlineMenuPrompt), "main>");
	cmd.CmdlineMenuItem = cmdlineMenuMain;
}

//##################################################################################################################

void Console_Menu_Config( void)
{
	NetworkCommandline_Config_Activate();
}

void Console_Menu_Main_Help( void)
{
	cmd.Console_sendString( (char*)
		"****************************************************************\r\n"
		"* Available commands are:                                      *\r\n"
		"*   config                     - Goto configuration menu       *\r\n"
		"*   help                       - displays available commands   *\r\n"
		"*   resetCounterDown           - reset Counter Down            *\r\n"
		"*   resetCounterUp             - reset Counter Up              *\r\n"
		"*   setEnablePID <0/1>         - enable auto rotor control     *\r\n"
		"*   setRotorDown <pos>         - move rotor down to pos degree *\r\n"
		"*   setRotorUp <pos>           - move rotor up to pos degree   *\r\n"
		"*   setRotorStop               - Stop rotor                    *\r\n"
		"*   showGpio                   - show GPIO values              *\r\n"
		"*   showStatus                 - show status                   *\r\n"
		"*   showStatusLine <s>         - update status every s ms      *\r\n"
		"*   stop                       - Stop rotor                    *\r\n"
		"****************************************************************\r\n");
}

void Console_Menu_ResetCounterDown( void)
{
	g_rototData[R_DOWN].CounterReferenced = true;
	EncoderReset(R_DOWN);
}

void Console_Menu_ResetCounterUp( void)
{
	g_rototData[R_UP].CounterReferenced = true;
	EncoderReset(R_UP);
}

void Console_Menu_SetEnablePID( void)
{
	g_enable_PID = cmd.cmdlineGetArgInt(1);
}

void Console_Menu_SetRotorUp( void)
{
	g_rototData[R_UP].TargetPositionInDegree = cmd.cmdlineGetArgDouble(1);
}

void Console_Menu_SetRotorDown( void)
{
	g_rototData[R_DOWN].TargetPositionInDegree = cmd.cmdlineGetArgDouble(1);
}

void Console_Menu_SetRotorStop( void)
{
	g_rototData[R_DOWN].MotorActive = false;
	g_rototData[R_UP].MotorActive = false;
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
	clientTelnet.print( "Rotor DOWN/UP:");

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
	clientTelnet.print( g_rototData[R_DOWN].MotorTurningCcw);
	clientTelnet.print( "/");
	clientTelnet.print( g_rototData[R_UP].MotorTurningCcw);
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
