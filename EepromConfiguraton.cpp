/*
 * EepromConfiguraton.cpp
 */

#include "Global.h"
#include <EEPROM.h>

// ----------------------------------------------------------------------------
// - 	EepromConfiguratonRead
// ----------------------------------------------------------------------------
void EepromConfiguratonRead()
{
	// To make sure there are settings, and they are YOURS!
	// If nothing is found it will use the default settings.
	if ( 	EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
			EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
			EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2] )
	{
		Serial.println("load EEprom");

		for (unsigned int t=0; t<sizeof(g_eepromStore); t++)
		{
			*((char*)&g_eepromStore + t) = EEPROM.read(CONFIG_START + t);
		}
	}
	else
	{
		Serial.println("EEprom empty, use default");
		for( uint16_t i=0; i< sizeof(g_eepromStore.version); i++)
		{
			g_eepromStore.version[i] = CONFIG_VERSION[i];
		}

		// default values
		g_eepromStore.NoDhcp = false;
		g_eepromStore.EncoderCounterScaler[0] = 1.0;
		g_eepromStore.EncoderCounterScaler[1] = 1.0;
	}
}

// ----------------------------------------------------------------------------
// - 	EepromConfiguratonWrite
// ----------------------------------------------------------------------------
void EepromConfiguratonWrite()
{
	Serial.println("save EEprom");

	for (unsigned int t=0; t<sizeof(g_eepromStore); t++)
	{
		EEPROM.write(CONFIG_START + t, *((char*)&g_eepromStore + t));
	}
}
