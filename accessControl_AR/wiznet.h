/*
 * wiznet.h
 *
 * Created: 08-05-2015 03:00:20 PM
 *  Author: Ajinkya
 */ 


#ifndef WIZNET_H_
#define WIZNET_H_

#include <Ethernet.h>



class wiznet{
	public:
		bool init();
		void serialPrintMAC(byte mac[6]);
	
		
};



#endif /* WIZNET_H_ */