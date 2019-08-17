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
void Console_Menu_Config_Help( void);
void Console_Menu_GotoMain( void);

static Commandline::tCmdlineMenu cmdlineMenuConfig[] =
{
	{"?", 							Console_Menu_Config_Help},
	{"configRead",					Console_Menu_ConfigRead},
	{"configWrite",					Console_Menu_ConfigWrite},
	{"configShow",					Console_Menu_ConfigShow},
	{"help",						Console_Menu_Config_Help},
	{"exit",						Console_Menu_GotoMain},
};

void NetworkCommandline_Config_Activate()
{
	cmd.CmdlineMenuAnzahl = sizeof(cmdlineMenuConfig) / sizeof(Commandline::tCmdlineMenu);
	strcpy ((char *) (cmd.CmdlineMenuPrompt), "config>");
	cmd.CmdlineMenuItem = cmdlineMenuConfig;
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

void Console_Menu_Config_Help( void)
{
	cmd.Console_sendString( (char*)
		"****************************************************************\r\n"
		"* Available commands are:                                      *\r\n"
		"*   configRead                 - Read Config from EEprom       *\r\n"
		"*   configWrite                - Write Config toEEprom         *\r\n"
		"*   configShow                 - Show Configuration            *\r\n"
		"*   help                       - displays available commands   *\r\n"
		"*   exit                       - Got Main Menu                 *\r\n"
		"****************************************************************\r\n");
}

void Console_Menu_GotoMain( void)
{
	NetworkCommandline_Main_Activate();
}
