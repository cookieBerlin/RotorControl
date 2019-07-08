/*
 * Global.h
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "Arduino.h"

#include "EepromConfiguraton.h"
#include "Display.h"
#include "Encoder.h"
#include "Network.h"

// ----------------------------------------------------------------------------
// -	Global type definitions
// ----------------------------------------------------------------------------
typedef struct tag_struct_RototData
{
	int32_t		counter;
} struct_RototData;

typedef struct tag_struct_EepromStore
{
	// This is for mere detection if they are your settings
	char					version[4];
} struct_EepromStore;

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// -	Global Values
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
#ifndef EXTERN
	#define EXTERN extern
#endif

EXTERN struct_EepromStore			g_eepromStore;
EXTERN volatile struct_RototData	g_rototData;


#endif /* GLOBAL_H_ */
