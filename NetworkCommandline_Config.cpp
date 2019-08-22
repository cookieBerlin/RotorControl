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
void Console_Menu_Config_UseDhcp( void);
void Console_Menu_Config_SetEncoderCounterScalerDown( void);
void Console_Menu_Config_SetEncoderCounterScalerUp( void);
void Console_Menu_Config_SetEncoderCounterOffsetDown( void);
void Console_Menu_Config_SetEncoderCounterOffsetUp( void);
void Console_Menu_GotoMain( void);

static Commandline::tCmdlineMenu cmdlineMenuConfig[] =
{
	{"?", 							Console_Menu_Config_Help},
	{"configRead",					Console_Menu_ConfigRead},
	{"configWrite",					Console_Menu_ConfigWrite},
	{"configShow",					Console_Menu_ConfigShow},
	{"help",						Console_Menu_Config_Help},
	{"setUseDhcp",					Console_Menu_Config_UseDhcp},
	{"setEncoderCounterScalerDown",	Console_Menu_Config_SetEncoderCounterScalerDown},
	{"setEncoderCounterScalerUp",	Console_Menu_Config_SetEncoderCounterScalerUp},
	{"setEncoderCounterOffsetDown",	Console_Menu_Config_SetEncoderCounterOffsetDown},
	{"setEncoderCounterOffsetUp",	Console_Menu_Config_SetEncoderCounterOffsetUp},
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

	clientTelnet.print( "\tEncoder Counter Offset Up / Down:      ");
	clientTelnet.print( g_eepromStore.EncoderCounterOffset[R_UP]);
	clientTelnet.print( " / ");
	clientTelnet.print( g_eepromStore.EncoderCounterOffset[R_DOWN]);
	clientTelnet.println( " degree");
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
		"*   setUseDhcp <s>             - Set DHCP on start 0=off, 1=on *\r\n"
		"*   setEncoderCounterScalerDown <v> - Set scaler in Down       *\r\n"
		"*   setEncoderCounterScalerUp <v>   - Set scaler in Up         *\r\n"
		"*   setEncoderCounterOffsetDown <v> - Set CounterOffset[degree]*\r\n"
		"*   setEncoderCounterOffsetUp <v>   - Set CounterOffset[degree]*\r\n"
		"*   exit                       - Got Main Menu                 *\r\n"
		"****************************************************************\r\n");
}

void Console_Menu_Config_UseDhcp( void)
{
	g_eepromStore.UseDhcp = cmd.cmdlineGetArgDouble(1);
}

void Console_Menu_Config_SetEncoderCounterScalerDown( void)
{
	g_eepromStore.EncoderCounterScaler[R_DOWN] = cmd.cmdlineGetArgDouble(1);
}

void Console_Menu_Config_SetEncoderCounterScalerUp( void)
{
	g_eepromStore.EncoderCounterScaler[R_UP] = cmd.cmdlineGetArgDouble(1);
}

void Console_Menu_Config_SetEncoderCounterOffsetDown( void)
{
	g_eepromStore.EncoderCounterOffset[R_DOWN] = cmd.cmdlineGetArgDouble(1);
}

void Console_Menu_Config_SetEncoderCounterOffsetUp( void)
{
	g_eepromStore.EncoderCounterOffset[R_UP] = cmd.cmdlineGetArgDouble(1);
}

void Console_Menu_GotoMain( void)
{
	NetworkCommandline_Main_Activate();
}
