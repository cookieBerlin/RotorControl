/*
 * Global.h
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "Arduino.h"

#include "Display.h"
#include "EepromConfiguraton.h"
#include "Gpio.h"
#include "Encoder.h"
#include "Network.h"
#include "NetworkROTCRL.h"
#include "NetworkCommandline.h"
#include "MotorControl.h"

 // Interval of calling central controller command, in milliseconds
#define CONTROL_INTERVALL 100

// ----------------------------------------------------------------------------
// -	CS for IC-MD counter
// ----------------------------------------------------------------------------
/// Chip Select for IC-MD Chip, encoder UP
#define PIN_CHIP_SELECT_IC_MD_UP		A4
/// Chip Select for IC-MD Chip, encoder Down
#define PIN_CHIP_SELECT_IC_MD_DOWN		A5

// ----------------------------------------------------------------------------
// -	GPIO for extrenal IOS
// ----------------------------------------------------------------------------
/// Input pin for KEY
#define PIN_GPIO_KEY					A10
/// Input pin for Rotor Up
#define PIN_GPIO_ROTOR_UP				A11
/// Input pin for Rotor LED
#define PIN_GPIO_ROTOR_LED				A9
/// Input pin for Rotor Down
#define PIN_GPIO_ROTOR_DOWN				A12
/// Input pin for Control CW
#define PIN_GPIO_CONTROL_CW				A13
/// Input pin for Control Led
#define PIN_GPIO_CONTROL_LED			A8
/// Input pin for Control CCW
#define PIN_GPIO_CONTROL_CCW			A14


// ----------------------------------------------------------------------------
// -	Pololu Dual VNH5019:		https://www.pololu.com/docs/0J49/all
// -	HW Modifications:
// -		open JP19 and connect JP19 Pin 2 with Arduino Pin 5
// ----------------------------------------------------------------------------
///		M1INA 	Motor 1 direction input A, is not the standard PIN ! (Conflict with Interrupt Input)
#define PIN_MD_INA1			5
///	 	M1INB 	Motor 1 direction input B
#define PIN_MD_INB1			4
///	 	M1EN/DIAG 	Motor 1 enable input/fault output
#define PIN_MD_EN1DIAG1		6
///		M1PWM 	Motor 1 speed input
#define PIN_MD_PWM1			9
///	 	M2INA 	Motor 2 direction input A
#define PIN_MD_INA2			7
///		M2INB 	Motor 2 direction input B
#define PIN_MD_INB2			8
///		M2PWM 	Motor 2 speed input, is not the standard PIN ! (Conflict with Ethernet Shield)
#define PIN_MD_PWM2			11		// PWM on pin 11 user Timer1 !!!
///		M2EN/DIAG 	Motor 2 enable input/fault output
#define PIN_MD_EN2DIAG2		12
///		M1CS 	Motor 1 current sense output
#define PIN_MD_CS1			A0
///		M2CS 	Motor 2 current sense output
#define PIN_MD_CS2			A1


// Index of Rotor UP in g_rototData array
#define R_DOWN 	0
// Index of Rotor Down in g_rototData array
#define R_UP	1


// ID of the settings block
#define CONFIG_VERSION "RC0"

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

typedef struct tag_struct_GpioData
{
	bool		Key;
	bool		RotorUp;
	bool		RotorLed;
	bool		RotorDown;
	bool		ControlCw;
	bool		ControlLed;
	bool		ControlCcw;
} struct_GpioData;


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
EXTERN volatile struct_GpioData		g_gpio;
EXTERN unsigned long 				g_ulShowSerialStatsLine;	// show status Line every [ms], 0 = off


#endif /* GLOBAL_H_ */
