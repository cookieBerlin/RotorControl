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
	DisplayTextLine(2, "IP: wait for DHCP...");

	// start the Ethernet connection:
	Serial.println("Trying to get an IP address using DHCP");

	if ( !g_eepromStore.NoDhcp && (Ethernet.begin(mac) == 0))
	{
		// initialize the Ethernet device not using DHCP:
		Ethernet.begin(mac, ip, myDns, gateway, subnet);
	}

	ip = Ethernet.localIP();

	// shop IP on display
	char buf[10];
	sprintf( buf, "IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	DisplayTextLine(2, buf);

	// shop IP on display
	Serial.print("IP address: ");
	Serial.println(buf);
}
