/*
 * accessControl.cpp
 *
 * Created: 30-04-2015 12:51:24 AM
 *  Author: Ajinkya
 */ 


#include "include.h"
extern "C"			//program in C language to maintain compatibility
{
	#include "uart.h"
}

int main(void)
{
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
	sei();
    while(1)
    {
		if(uart_available())
		{
			uart_putc(uart_getc());
		}
    }
}