/*
 * NetworkROTCRL.h
 */

#ifndef NETWORKROTCRL_H_
#define NETWORKROTCRL_H_

#include <Ethernet2.h>

class NetworkROTCRL
{
	public:
		NetworkROTCRL(uint16_t Port, double *ActualAzimuth, double *ActualElevation, double *TargetAzimuth, double *TargetElevation);
		void HandleCommunication();

	private:
		typedef enum tag_enum_NetWorkCommunication_StateMaschine
		{
			eWaitForNewClient,
			eWaitForData,
			eClientDisconnect,
		} enum_NetWorkCommunication_StateMaschine;

		// Configure Server Port to 4533, default of rotclt
		EthernetServer * serverROTCRL;

		String readString = ""; //string for fetching data form Client
		EthernetClient clientROTCRL;

		enum_NetWorkCommunication_StateMaschine eState;

		double *actualAzimuth;
		double *actualElevation;
		double *targetAzimuth;
		double *targetElevation;

		void DecodeClientData(void);
};

#endif /* NETWORKROTCRL_H_ */
