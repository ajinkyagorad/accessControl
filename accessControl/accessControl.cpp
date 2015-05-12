/*
 * accessControl.cpp
 *
 * Created: 30-04-2015 12:51:24 AM
 *  Author: Ajinkya
 */ 


#include "include.h"
#include "systime.h"		//interrupt based

extern "C"	{		//program in C language to maintain compatibility	//otherwise will give undefined reference
	#include "spi.h"
	#include "uart.h"		//interupt based
	}
#include "w5100.h"

//Assign I/O stream to UART


#define NULL  0
bool getRFID(char* id)
{
	
	unsigned char index=0;
	long time=systime::getSysTime();
	//for checking that input arrives in specific time
	while((systime::getSysTime()-time)<20)
	{
		if(uart_available())
		{
			id[index]=uart_getc();
			index++;
		}
	}
	if(index==12)return true;
	else return false;
}
int strlen( char * str)
{
	int i;
	for( i=0;str[i]!=0;i++);
	return i;
}
unsigned char buf[MAX_BUF];
int main(void)
{
	
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
	spi_init();
	w5100 ethernet;
	ethernet.init();
	ethernet.socket(0,MR_TCP,80);
	systime::sysTimeInit();
	sei();
	char * id=NULL;
	unsigned int sockStatus,rsize;
    while(1)
    {
		
	//	if(getRFID(id))
		//{
	//		for(int i=0;i<12;i++)uart_putc(id[i]);
			//uart_puts_p("\n\r");
	//	}
		sockStatus=ethernet.getStatus();
		switch(sockStatus)
		{
			case SOCK_CLOSED:
				if(ethernet.socket(0,MR_TCP,80)>0)
				{
					//if closed then listen to a connection
					if(ethernet.listen(0)==0)
					_delay_ms(1); //delay and end
					
				}
				break;
			case SOCK_ESTABLISHED:
				rsize=ethernet.recv_size();
				if(rsize>0)
				{
					if(ethernet.recv(0,buf,rsize)<=0)break;
					//create response header
					strcpy_P((char *)buf,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n"));
					strcat_P((char *)buf,PSTR("<html><body><span style=\"color:#0000A0\">\r\n"));
					strcat_P((char *)buf,PSTR("<h1>RFID</h1>\r\n"));
					
					//send response header
					if(ethernet.send(0,buf,strlen((char*)buf))<=0)break;
					ethernet.disconnect(0);
					
				}
				break;
			default:
			// Force to close the socket
				ethernet.close(0);
				break;
		}
		
    }
}