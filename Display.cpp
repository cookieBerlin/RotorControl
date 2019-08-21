/*
 * Display.cpp
 */

#include "Global.h"
#include <LiquidCrystal_I2C.h>

// ----------------------------------------------------------------------------
// - 	Globals
// ----------------------------------------------------------------------------

// Set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 20, 4);


const int SymbolArrowUp = 0;
uint8_t LcdCharArrowUp[8]  = {
		0b00100,
		0b01110,
		0b10101,
		0b00100,
		0b00100,
		0b00100,
		0b00100};

const int SymbolArrowDown = 1;
uint8_t LcdCharArrowDown[8]  = {
		0b00100,
		0b00100,
		0b00100,
		0b00100,
		0b10101,
		0b01110,
		0b00100};

const int SymbolDegree = 2;
uint8_t LcdCharDegree[8]  = {
		0b01110,
		0b01010,
		0b01110,
		0b00000,
		0b00000,
		0b00000,
		0b0000};

// ----------------------------------------------------------------------------
// - 	SetupDisplay
// ----------------------------------------------------------------------------
void SetupDisplay()
{
	lcd.init();

	lcd.backlight(); 			// Turn on the blacklight
	lcd.noAutoscroll();			// disable auto scroll
	lcd.leftToRight();			// activate left to right writing
	lcd.cursor_off();			// hide Cursor
	lcd.clear();				// clear display

	lcd.createChar(SymbolArrowUp, LcdCharArrowUp);
	lcd.createChar(SymbolArrowDown, LcdCharArrowDown);
	lcd.createChar(SymbolDegree, LcdCharDegree);
}


// ----------------------------------------------------------------------------
// - 	DisplayClear
// ----------------------------------------------------------------------------
void DisplayClear()
{
	lcd.clear();
}

// ----------------------------------------------------------------------------
// - 	DisplayTextLine
// ----------------------------------------------------------------------------
void DisplayTextLine(const int line, const char text[])
{
	// clear line
	lcd.setCursor(0, line);
	lcd.print("                    ");

	// write line
	lcd.setCursor(0, line);
	lcd.print(text);
}

// ----------------------------------------------------------------------------
// - 	DisplayShowStatusOfRotor
// ----------------------------------------------------------------------------
void DisplayShowStatusOfRotor(const int rotorId)
{
	const int line = (rotorId==R_UP) ? 0 : 2;

	// goto first line
	lcd.setCursor(0, line);

	// show arrow
	lcd.write(rotorId);

	// show degree
	lcd.print(" ");
	lcd.print(g_rototData[rotorId].CounterInDegree, 1);
	lcd.write(SymbolDegree);	// show Degree

	// show counter
	lcd.print("(");
	lcd.print(g_rototData[rotorId].CounterInSteps);
	lcd.print(")     ");

	// goto second line
	lcd.setCursor(0, line+1);

	// show Reference need warning
	if(!g_rototData[rotorId].CounterReferenced)
	{
		lcd.print("!REF! ");
	}

	// show motor status
	if( !g_rototData[rotorId].MotorActive)
	{
		lcd.print("Motor Stopped ");
	}
	else if (g_rototData[rotorId].MotorTurningCcw)
	{
		lcd.print("Motor Turn CCW ");
	}
	else
	{
		lcd.print("Motor Turn CW  ");
	}

	// show curent
	lcd.print("(");
	lcd.print(g_rototData[rotorId].MotorCurrentInMa);
	lcd.print(" mA)");
}

// ----------------------------------------------------------------------------
// - 	DisplayShowStatus
// ----------------------------------------------------------------------------
void DisplayShowStatus()
{
	DisplayShowStatusOfRotor(R_DOWN);
	DisplayShowStatusOfRotor(R_UP);
}
