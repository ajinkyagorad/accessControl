#include <SD.h>

#include <GSM.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>



/*
 * accessControl_AR.ino
 *
 * Created: 5/8/2015 5:20:20 AM
 * Author: Ajinkya
 */ 
#include "RFID.h"
#include "wiznet.h"


wiznet ether;
EthernetClient client;
void setup()
{

	  /* add setup code here, setup code runs once when the processor starts */
	  Serial.begin(9600);
	  RFID::init();
	  ether.init();
	  
	   Serial.println("Initialised");
	 // RFID::init();d
	 Serial.println("Waiting for connection..");
	 while(client.connect("10.3.192.78",8080)!=1);
	 Serial.println("made connection :>");
	 client.stop();
	 
}
RFID::id rfid;
void loop()
{
		//wait for first connection  is made :>
		//loop {
		//if rfid is read , transmit a request;
		//		if connection  problem print :connection problem
		//look for response while client is connected.
		//print response	}
		
		RFID::getID(rfid);
		if(rfid.isValid==true)
		{
			String data="id="+String(rfid.buf);
			client.connect("10.3.192.78",8080);
			client.print("GET /usingphp.php?");client.print(data);client.println(" HTTP/1.1");
			client.println("Host: 10.3.192.78");
			client.println("Connection: close");
			client.println();
			while(client.connected())
			{
				if(client.available())
				{
					Serial.print((char)(client.read()));
				}
			}
			client.stop();
		}
		else
		{
			//Serial.println("rfid data not read");
		}
}
