/*
 * Network.c
 *
 *  See: http://hamlib.sourceforge.net/manuals/hamlib.html#rotctld-commands
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
#define DEBUG_CLIENT_COMMANDS


void NetworkROTCRL_Init()
{
	Serial.println("Start Network ROTCRL");

	// start listening for clients
	serverROTCRL.begin();
}

// ----------------------------------------------------------------------------
void decodeClientData()
{
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
      }
      else
      {
#ifdef DEBUG_CLIENT_COMMANDS
//        Serial.println(" Command: '" + readString + "'");
//        Serial.print("\tiFirstSpace: ");
//        Serial.print( iFirstSpace);
//        Serial.println(" ");
//        Serial.print("\tiSecondSpace:" );
//        Serial.print( iSecondSpace);
//        Serial.println(" ");
        Serial.print(g_rototData[R_DOWN].TargetPositionInDegree);
        Serial.print(" ");
        Serial.print(g_rototData[R_UP].TargetPositionInDegree);
        Serial.println(" ");
#endif
    	g_rototData[R_DOWN].TargetPositionInDegree = readString.substring(iFirstSpace).toFloat();
//        if( g_rototData[R_DOWN].TargetPositionInDegree > 180) g_rototData[R_DOWN].TargetPositionInDegree = 180;
//        if( g_rototData[R_DOWN].TargetPositionInDegree < -180) g_rototData[R_DOWN].TargetPositionInDegree = -180;

        g_rototData[R_UP].TargetPositionInDegree = readString.substring(iSecondSpace).toFloat();
//        if( g_rototData[R_UP].TargetPositionInDegree > 90) g_rototData[R_UP].TargetPositionInDegree = 90;
//        if( g_rototData[R_UP].TargetPositionInDegree < -90) g_rototData[R_UP].TargetPositionInDegree = -90;

        sAnswer += "RPRT -4";
        //sAnswer += "\n";
      }
  }
  else if( readString.startsWith("p") ||
           readString.startsWith("get_pos"))
  {
#ifdef DEBUG_CLIENT_COMMANDS
    Serial.println("Command: get_pos");
#endif
    sAnswer += g_rototData[R_DOWN].CounterInDegree;
    sAnswer += "\n";
    sAnswer += g_rototData[R_UP].CounterInDegree;
    sAnswer += "\n";
    Serial.println(sAnswer);
  }
  else if( readString.startsWith("-") ||
		   readString.startsWith("\\dump_state"))
  {
#ifdef DEBUG_CLIENT_COMMANDS
    Serial.println("Command: dump_state");
#endif
    sAnswer += "1\n"; // protocol Version
    sAnswer += "23\n"; // Unknown
    sAnswer += "-180.0\n"; // Min_AZ
    sAnswer += "+180.0\n"; // Max_AZ
    sAnswer += "0.0\n"; // Min_EL
    sAnswer += "90.0\n"; // Max_EL
  }
  else if( readString.startsWith("_") ||
		   readString.startsWith("get_info"))
  {
#ifdef DEBUG_CLIENT_COMMANDS
    Serial.println("Command: get_info");
#endif
    sAnswer += "FTM25/6\n"; // Model Name.
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
      sAnswer += "RPRT -4";
  }

  if( sAnswer.length() > 0 )
  {
	  clientROTCRL.print(sAnswer);
  }

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

