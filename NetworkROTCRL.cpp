/*
 * Network.c
 *
 *  Created on: 14.05.2017
 *      Author: cookie
 */

#include "Global.h"
#include "Arduino.h"
#include <SPI.h>
#include <Ethernet2.h>
#include <WString.h>


// ----------------------------------------------------------------------------
// - 	Network Service
// ----------------------------------------------------------------------------
// Configure Server Port to 4533, default of rotclt
EthernetServer serverROTCRL(4533);

String readString = ""; //string for fetching data form Client
EthernetClient clientROTCRL;


// show Network Commands in Serial1
//#define DEBUG_CLIENT_COMMANDS


void NetworkROTCRL_Init()
{
	Serial.println("Start Network ROTCRL");

	// start listening for clients
	serverROTCRL.begin();
}

// ----------------------------------------------------------------------------
void decodeClientData()
{
  int iRetVal = 0;
  String sAnswer = "";

  if( readString.startsWith("P") ||
      readString.startsWith("set_pos"))
  {
#ifdef DEBUG_CLIENT_COMMANDS
      Serial.println("Command: set_pos");
#endif

      // search for spaces
      int iFirstSpace = readString.indexOf(' ');
      int iSecondSpace = readString.indexOf(' ', (iFirstSpace + 5));

      if( (iFirstSpace == -1) ||
          (iSecondSpace == -1) )
      {
        Serial.println("Parser Error in RX Command: '" + readString + "'");
        Serial.print("\tiFirstSpace: ");
        Serial.print( iFirstSpace);
        Serial.println(" ");
        Serial.print("\tiSecondSpace:" );
        Serial.print( iSecondSpace);
        Serial.println(" ");
        iRetVal = -2;
      }
      else
      {
    	g_rototData[0].TargetPositionInDegree = readString.substring(iFirstSpace).toFloat();
//        if( g_rototData[0].TargetPositionInDegree > 180) g_rototData[0].TargetPositionInDegree = 180;
//        if( g_rototData[0].TargetPositionInDegree < -180) g_rototData[0].TargetPositionInDegree = -180;

        g_rototData[1].TargetPositionInDegree = readString.substring(iSecondSpace).toFloat();
//        if( g_rototData[1].TargetPositionInDegree > 90) g_rototData[1].TargetPositionInDegree = 90;
//        if( g_rototData[1].TargetPositionInDegree < -90) g_rototData[1].TargetPositionInDegree = -90;

//        Serial.print("Command: set_pos");
//        Serial.println(" Command: '" + readString + "'");
//        Serial.print("\tiFirstSpace: ");
//        Serial.print( iFirstSpace);
//        Serial.println(" ");
//        Serial.print("\tiSecondSpace:" );
//        Serial.print( iSecondSpace);
//        Serial.println(" ");
//
//        Serial.print(g_sControlAzimuth.dPositionTaret);
//        Serial.print(" ");
//        Serial.print(g_sControlElevation.dPositionTaret);
//        Serial.println(" ");
      }
  }
  else if( readString.startsWith("p") ||
           readString.startsWith("get_pos"))
  {
#ifdef DEBUG_CLIENT_COMMANDS
    Serial.println("Command: get_pos");
#endif
    sAnswer += g_rototData[0].CounterInDegree;
    sAnswer += "\n";
    sAnswer += g_rototData[1].CounterInDegree;
    sAnswer += "\n";
  }
  else if( readString.startsWith("Q") ||
           readString.startsWith("q") ||
           readString.startsWith("exit rotctl"))
  {
#ifdef DEBUG_CLIENT_COMMANDS
      Serial.println("Command: Quit");
#endif
      clientROTCRL.stop();
  }
  else
  {
      Serial.print("\r\nUnknown Command string: '");
      Serial.print(readString);
      Serial.print("'\r\n");
      iRetVal = -1;
  }

  // send Status to Client
  sAnswer += "RPRT ";
  sAnswer += iRetVal;
  sAnswer += "\n";

  clientROTCRL.print(sAnswer);

  // clean Buffer
  readString = "";
}

void NetworkROTCRL_handleCommunication()
{
	typedef enum tag_enum_NetWorkCommunication_StateMaschine
	{
		eWaitForNewClient,
		eWaitForData,
		eClientDisconnect,
	} enum_NetWorkCommunication_StateMaschine;

	static enum_NetWorkCommunication_StateMaschine eState = eWaitForNewClient;

	switch ( eState )
	{
		case eWaitForNewClient:
			// wait for a new clientROTCRL:
			clientROTCRL = serverROTCRL.available();
			if (clientROTCRL)
			{
				Serial.println("Client Connected");
		    	readString = "";
		    	eState = eWaitForData;
			}
			break;
		case eWaitForData:
			if( clientROTCRL.connected() == 0 )
			{
		    	eState = eClientDisconnect;
			}
			else if (clientROTCRL.available())
			{
				char c = clientROTCRL.read();

				//read char by char from Client request, check buffer size
				if (readString.length() < 100)
				{
					//store characters to string
					readString += c;
				}

				// if HTTP request has ended
				if (c == '\n')
				{
					decodeClientData();
				}
			}
			break;
		case eClientDisconnect:
		    Serial.println("Client Disconnected");
		    clientROTCRL = 0;
		    eState = eWaitForNewClient;
			break;
	}
}

