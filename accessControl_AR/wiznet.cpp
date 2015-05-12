/*
 * wiznet.cpp
 *
 * Created: 08-05-2015 03:00:04 PM
 *  Author: Ajinkya
 */ 

#ifndef WIZNET_CPP_
#define WIZNET_CPP_

#include "wiznet.h"
//*******************************************
//modify the Following accordingly:

/*MAC ADDRESS OF THE ETHERNET SHIELD*/
byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/*IP Address of this Device : Default IP Address if DHCP Fails */
IPAddress ip(10,3,192,177);

/*DNS Server, Gateway, Subnet Mask */
IPAddress dnsip(10,200,1,11);
IPAddress gateway(10,3,200,250);
IPAddress subnet(255,255,224,0);

//ethernet client
///EthernetClient client;

//Server & Port to make contact
char server[] = "www.page.com";
//comment above and uncomment following if server's ip address is given
//IPAddress server(10,3,0,2);
uint16_t port=80;


//*********************************************************
bool wiznet::init()
{
	//initialise using manual settings 
	Serial.println("Initialising Ethernet...");
	Ethernet.begin(mac,ip,dnsip,gateway,subnet);
	//display relevant information : serial
	Serial.print("MAC : ");	serialPrintMAC(mac);
	Serial.print("IP : ");	Serial.println(Ethernet.localIP());
	Serial.print("DNS : ");	Serial.println(Ethernet.dnsServerIP());
	Serial.print("GATEWAY : ");	Serial.println(Ethernet.gatewayIP());
	Serial.print("SUBNET : ");	Serial.println(Ethernet.subnetMask());
	

}
void wiznet::serialPrintMAC(byte mac[6])
{
	for(char i=0;i<5;i++)
	{
		Serial.print(mac[i],HEX);
		Serial.print(":");
	}
	Serial.println(mac[5]);
}


#endif