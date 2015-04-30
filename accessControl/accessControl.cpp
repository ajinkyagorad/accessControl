/*
 * accessControl.cpp
 *
 * Created: 30-04-2015 12:51:24 AM
 *  Author: Ajinkya
 */ 


#include "include.h"
#include "systime.h"

extern "C"			//program in C language to maintain compatibility	//otherwise will give undefined reference
{
	
	
	#include "spi.h"
	#include "uart.h"
	
}

//Assign I/O stream to UART


#define NULL  0
char* getRFID(void)
{
	char id[12];
	unsigned char index=0;
	long time=systime::getSysTime();
	//for checking that input arrives in specific time
	while((systime::getSysTime()-time)<100)
	{
		if(uart_available())
		{
			id[index]=uart_getc();
			index++;
		}
	}
	if(index==12)return id;
	else return NULL;
}
int main(void)
{
	
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
	spi_init();
	systime::sysTimeInit();
	sei();
	char * id=NULL;
    while(1)
    {
		
		id=getRFID();
		if(id==NULL)
		{
			//uart_puts_P("Error Reading RFID\n\r");
		}
		else
		{
			for(int i=0;i<12;i++)uart_putc(id[i]);
			//uart_puts_p("\n\r");
		}
    }
}