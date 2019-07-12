/*
 * Network.cpp
 */

#include "Global.h"

#include <SPI.h>
#include <Ethernet2.h>

// ----------------------------------------------------------------------------
// - 	Network Interface
// ----------------------------------------------------------------------------
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(192, 168, 1, 177);
IPAddress myDns(192,168,1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);


// ----------------------------------------------------------------------------
// - 	SetupNetwork
// ----------------------------------------------------------------------------
void SetupNetwork()
{
	int resultDhcp = 0;

	// start the Ethernet connection:
	if ( g_eepromStore.UseDhcp )
	{
		DisplayTextLine(2, "IP: wait for DHCP...");
		Serial.println("Trying to get an IP address using DHCP");
		resultDhcp = Ethernet.begin(mac);
		if( resultDhcp == 0)
		{
			DisplayTextLine(2, "IP: wait for DHCP...FAIL");
			delay(1500);
		}
	}

	if (resultDhcp == 0)
	{
		DisplayTextLine(2, "IP: set static...");
		Serial.println("Configure static IP address");

		// initialize the Ethernet device not using DHCP:
		Ethernet.begin(mac, ip, myDns, gateway, subnet);
	}

	ip = Ethernet.localIP();

	// shop IP on display
	char buf[10];
	sprintf( buf, "IP: %d.%d.%d.%d %s", ip[0], ip[1], ip[2], ip[3], ((resultDhcp == 0)? "STAT": "DHCP"));
	DisplayTextLine(2, buf);

	// shop IP on display
	Serial.print("IP address: ");
	Serial.println(buf);
}
