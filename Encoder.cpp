/*
 * Encoder.cpp
 */

#include "Global.h"
#include "DriverIcMd.h"

DriverIcMd *driverIcMd[NumberOfRotors] = {NULL};

// ----------------------------------------------------------------------------
// - 	SetupEncoder
// ----------------------------------------------------------------------------
void SetupEncoder()
{
	char buf[21];

	driverIcMd[0] = new DriverIcMd( PIN_CHIP_SELECT_IC_MD_UP);
	driverIcMd[1] = new DriverIcMd( PIN_CHIP_SELECT_IC_MD_DOWN);

	for( int i=0; i<NumberOfRotors; i++)
	{
		sprintf(buf, "Init Counter %d", i);
		DisplayTextLine(3, buf);

		// init counter
		int result = driverIcMd[i]->CheckDevice();
		if( result == 0)
		{
			sprintf(buf, "Init Counter %d OK", i);
			DisplayTextLine(3, buf);
			delay(250);
		}
		else
		{
			sprintf(buf, "Init Counter %d Fail", i);
			DisplayTextLine(3, buf);
			while(true);
		}

		// default Values
		driverIcMd[i]->SetCounterMode(DriverIcMd::CounterMode::Counter0_32Bit);
		driverIcMd[i]->SetIndexSignalZMode(DriverIcMd::IndexSignalZMode::A1B1);
		driverIcMd[i]->SetInputMode( DriverIcMd::Differential);
		driverIcMd[i]->SetDifferentialInputMode( DriverIcMd::RS422);

		driverIcMd[i]->SetIndexClearedCounter( i, DriverIcMd::NotCleanedByZ);
		driverIcMd[i]->SetCountingDirection( i, DriverIcMd::CCWpositive);
		driverIcMd[i]->SetIndexInvertedMode( i, DriverIcMd::NonInverted);

		// reset Counter
		DriverIcMd::UnionInstructionByte instruction = {0};
		instruction.bits.ZCen_EnableZeroCodification = 1;
		instruction.bits.ABres0_ResetCounter0 = 1;
		instruction.bits.ABres1_ResetCounter1 = 1;
		instruction.bits.ABres2_ResetCounter2 = 1;
		driverIcMd[i]->SendInstruction(instruction);
	}
}

// ----------------------------------------------------------------------------
// - 	EncoderUpdate
// ----------------------------------------------------------------------------
void EncoderUpdate()
{
	for( int i=0; i<NumberOfRotors; i++)
	{
		DriverIcMd::UnionCounterValues counter = driverIcMd[i]->GetCounter();

		// read Data Counter Value
		g_rototData[i].CounterInSteps = counter.Counter0_32Bit.Counter0;
		g_rototData[i].CounterInDegree = g_rototData[i].CounterInSteps * g_eepromStore.EncoderCounterScaler[i];

//		Serial.print( "Counter=");
//		Serial.print( counter.Counter0_32Bit.Counter0, DEC);
//		Serial.print( "\tNERR=0b");
//		Serial.print( counter.Counter0_32Bit.NERR, BIN);
//		Serial.print( "\tNWARN=0b");
//		Serial.print( counter.Counter0_32Bit.NWARN, BIN);
//
//
//		if( (counter.Counter0_32Bit.NERR != 0) ||
//		    (counter.Counter0_32Bit.NWARN != 0))
//		{
//			DriverIcMd::IcMdStatus status = driverIcMd[i]->GetStatus();
//
//			Serial.print( "\t");
//			Serial.print( "\tStatus 0x48=0b");
//			Serial.print( status.reg0.bByte, BIN);
//			Serial.print( "\tStatus 0x49=0b");
//			Serial.print( status.reg1.bByte, BIN);
//			Serial.print( "\tStatus 0x4A=0b");
//			Serial.print( status.reg2.bByte, BIN);
//		}
//
//		Serial.println( "");
	}
}
