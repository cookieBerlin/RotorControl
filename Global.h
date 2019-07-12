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
#include "MotorControl.h"
#include "NetworkROTCRL.h"

 // Interval of calling central controller command, in milliseconds
#define CONTROL_INTERVALL 100

/// Chip Select for IC-MD Chip, encoder UP
#define PIN_CHIP_SELECT_IC_MD_UP		6

/// Chip Select for IC-MD Chip, encoder Down
#define PIN_CHIP_SELECT_IC_MD_DOWN		7

// ID of the settings block
#define CONFIG_VERSION "MS0"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

// Number of Rotors, unly 2 are supported
#define NumberOfRotors	2

// ----------------------------------------------------------------------------
// -	Global type definitions
// ----------------------------------------------------------------------------
typedef struct tag_struct_RototData
{
	int32_t		CounterInSteps;
	double		CounterInDegree;
	bool		CounterReferenced;
	bool		MotorActive;
	bool		MotorTurningLeft;
	double		TargetPositionInDegree;
} struct_RototData;

typedef struct tag_struct_EepromStore
{
	// This is for mere detection if they are your settings
	char					version[4];
	// Activate DHCP on startup
	bool					UseDhcp;
	// hold Scaler for Encoder count -> Degree
	double					EncoderCounterScaler[NumberOfRotors];
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
EXTERN volatile struct_RototData	g_rototData[NumberOfRotors];


#endif /* GLOBAL_H_ */
