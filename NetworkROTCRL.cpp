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


// show Network Commands in Serial1
//#define DEBUG_CLIENT_COMMANDS

// ----------------------------------------------------------------------------
NetworkROTCRL::NetworkROTCRL(uint16_t Port, double *ActualAzimuth, double *ActualElevation, double *TargetAzimuth, double *TargetElevation)
{
	Serial.print("Start Network ROTCRL(Port ");
	Serial.print(Port);
	Serial.println(")");

	// start listening for clients
	serverROTCRL = new EthernetServer(Port);
	//serverROTCRL->
	serverROTCRL->begin();

	this->eState = eWaitForNewClient;
	this->actualAzimuth = ActualAzimuth;
	this->actualElevation = ActualElevation;
	this->targetAzimuth = TargetAzimuth;
	this->targetElevation = TargetElevation;
}

// ----------------------------------------------------------------------------
void NetworkROTCRL::DecodeClientData(void)
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
      int iSecondSpace = readString.indexOf(' ', (iFirstSpace + 2));

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
        Serial.println(" Command: '" + readString + "'");
        Serial.print("\tiFirstSpace: ");
        Serial.print( iFirstSpace);
        Serial.println(" ");
        Serial.print("\tiSecondSpace:" );
        Serial.print( iSecondSpace);
        Serial.println(" ");
#endif
        if( targetAzimuth != NULL)
        {
        	*targetAzimuth = readString.substring(iFirstSpace).toFloat();
        }
        if( targetElevation != NULL)
        {
        	*targetElevation = readString.substring(iSecondSpace).toFloat();
        }

#ifdef DEBUG_CLIENT_COMMANDS
        Serial.print(*targetAzimuth);
        Serial.print(" ");
        Serial.print(*targetElevation);
        Serial.println(" ");
#endif
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
    sAnswer += (actualAzimuth!=NULL) ? *actualAzimuth : 0;
    sAnswer += "\n";
    sAnswer += (actualElevation!=NULL) ? *actualElevation : 0;
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
    sAnswer += "0.0\n"; // Min_AZ
    sAnswer += "+360.0\n"; // Max_AZ
    sAnswer += "0.0\n"; // Min_EL
    sAnswer += "0.0\n"; // Max_EL
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
  else if( readString.startsWith("help"))
  {
#ifdef DEBUG_CLIENT_COMMANDS
      Serial.println("Command: Help");
#endif
      sAnswer += "Commands:\r\n";
      sAnswer += "\t P / set_pos\r\n";
      sAnswer += "\t p / get_pos\r\n";
      sAnswer += "\t _ / get_info\r\n";
      sAnswer += "\t - / dump_state";
      sAnswer += "\t Q / q / exit rotctl\r\n";
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

void NetworkROTCRL::HandleCommunication()
{
	switch ( eState )
	{
		case eWaitForNewClient:
			// wait for a new clientROTCRL:
			clientROTCRL = serverROTCRL->available();
			if (clientROTCRL)
			{
				Serial.println("Network ROTCRL: Client Connected");
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
					DecodeClientData();
				}
			}
			break;
		case eClientDisconnect:
		    Serial.println("Network ROTCRL: Client Disconnected");
		    clientROTCRL = 0;
		    eState = eWaitForNewClient;
			break;
	}
}

