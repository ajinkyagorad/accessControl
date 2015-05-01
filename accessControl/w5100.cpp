/*
 * w5100.cpp
 *
 * Created: 01-05-2015 04:39:19 AM
 *  Author: Ajinkya
 */ 

//source : http://www.ermicro.com/blog/?p=1773
#include "w5100.h"
w5100::w5100()
{
	
}

void w5100::init(void)
{
	// Ethernet Setup
	unsigned char mac_addr[] = {0x00,0x16,0x36,0xDE,0x58,0xF6};
	unsigned char ip_addr[] = {192,168,2,10};
	unsigned char sub_mask[] = {255,255,255,0};
	unsigned char gtw_addr[] = {192,168,2,1};

	// Setting the Wiznet W5100 Mode Register: 0x0000
	SPI_Write(MR,0x80);            // MR = 0b10000000;

	// Setting the Wiznet W5100 Gateway Address (GAR): 0x0001 to 0x0004
	SPI_Write(GAR + 0,gtw_addr[0]);
	SPI_Write(GAR + 1,gtw_addr[1]);
	SPI_Write(GAR + 2,gtw_addr[2]);
	SPI_Write(GAR + 3,gtw_addr[3]);

	// Setting the Wiznet W5100 Source Address Register (SAR): 0x0009 to 0x000E
	SPI_Write(SAR + 0,mac_addr[0]);
	SPI_Write(SAR + 1,mac_addr[1]);
	SPI_Write(SAR + 2,mac_addr[2]);
	SPI_Write(SAR + 3,mac_addr[3]);
	SPI_Write(SAR + 4,mac_addr[4]);
	SPI_Write(SAR + 5,mac_addr[5]);

	// Setting the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
	SPI_Write(SUBR + 0,sub_mask[0]);
	SPI_Write(SUBR + 1,sub_mask[1]);
	SPI_Write(SUBR + 2,sub_mask[2]);
	SPI_Write(SUBR + 3,sub_mask[3]);

	// Setting the Wiznet W5100 IP Address (SIPR): 0x000F to 0x0012
	SPI_Write(SIPR + 0,ip_addr[0]);
	SPI_Write(SIPR + 1,ip_addr[1]);
	SPI_Write(SIPR + 2,ip_addr[2]);
	SPI_Write(SIPR + 3,ip_addr[3]);

	// Setting the Wiznet W5100 RX and TX Memory Size (2KB),
	SPI_Write(RMSR,NET_MEMALLOC);
	SPI_Write(TMSR,NET_MEMALLOC);
}
unsigned char w5100::SPI_Read(unsigned int addr)
{
	 // Activate the CS pin
	 SPI_PORT &= ~(1<<SPI_CS);

	 // Start Wiznet W5100 Read OpCode transmission
	 SPDR = WIZNET_READ_OPCODE;

	 // Wait for transmission complete
	 while(!(SPSR & (1<<SPIF)));

	 // Start Wiznet W5100 Address High Bytes transmission
	 SPDR = (addr & 0xFF00) >> 8;

	 // Wait for transmission complete
	 while(!(SPSR & (1<<SPIF)));

	 // Start Wiznet W5100 Address Low Bytes transmission
	 SPDR = addr & 0x00FF;

	 // Wait for transmission complete
	 while(!(SPSR & (1<<SPIF)));

	 // Send Dummy transmission for reading the data
	 SPDR = 0x00;

	 // Wait for transmission complete
	 while(!(SPSR & (1<<SPIF)));

	 // CS pin is not active
	 SPI_PORT |= (1<<SPI_CS);

	 return(SPDR);
}

void w5100::SPI_Write(unsigned int addr, unsigned char data)
{
	 // Activate the CS pin
	 SPI_PORT &= ~(1<<SPI_CS);

	 // Start Wiznet W5100 Write OpCode transmission
	 SPDR = WIZNET_WRITE_OPCODE;

	 // Wait for transmission complete
	 while(!(SPSR & (1<<SPIF)));

	 // Start Wiznet W5100 Address High Bytes transmission
	 SPDR = (addr & 0xFF00) >> 8;

	 // Wait for transmission complete
	 while(!(SPSR & (1<<SPIF)));

	 // Start Wiznet W5100 Address Low Bytes transmission
	 SPDR = addr & 0x00FF;

	 // Wait for transmission complete
	 while(!(SPSR & (1<<SPIF)));

	 // Start Data transmission
	 SPDR = data;

	 // Wait for transmission complete
	 while(!(SPSR & (1<<SPIF)));

	 // CS pin is not active
	 SPI_PORT |= (1<<SPI_CS);
}

void w5100::close(unsigned char sock)
{
	
	if (sock != 0) return;

	// Send Close Command
	SPI_Write(S0_CR,CR_CLOSE);

	// Waiting until the S0_CR is clear
	while(SPI_Read(S0_CR));
}
void w5100::disconnect(unsigned char sock)
{
	if (sock != 0) return;

	// Send Disconnect Command
	SPI_Write(S0_CR,CR_DISCON);

	// Wait for Disconecting Process
	while(SPI_Read(S0_CR));
}

unsigned char w5100::socket(unsigned char sock, unsigned char eth_protocol, unsigned int tcp_port)
{
	 uint8_t retval=0;

	 if (sock != 0) return retval;

	 // Make sure we close the socket first
	 if (SPI_Read(S0_SR) == SOCK_CLOSED) {
		 close(sock);
	 }

	 // Assigned Socket 0 Mode Register
	 SPI_Write(S0_MR,eth_protocol);

	 // Now open the Socket 0
	 SPI_Write(S0_PORT,((tcp_port & 0xFF00) >> 8 ));
	 SPI_Write(S0_PORT + 1,(tcp_port & 0x00FF));
	 SPI_Write(S0_CR,CR_OPEN);                   // Open Socket

	 // Wait for Opening Process
	 while(SPI_Read(S0_CR));

	 // Check for Init Status
	 if (SPI_Read(S0_SR) == SOCK_INIT)
	 retval=1;
	 else
	 close(sock);

	 return retval;
}

unsigned  char w5100::listen(unsigned char sock)
{
	 uint8_t retval = 0;

	 if (sock != 0) return retval;

	 if (SPI_Read(S0_SR) == SOCK_INIT) {
		 // Send the LISTEN Command
		 SPI_Write(S0_CR,CR_LISTEN);

		 // Wait for Listening Process
		 while(SPI_Read(S0_CR));

		 // Check for Listen Status
		 if (SPI_Read(S0_SR) == SOCK_LISTEN)
		 retval=1;
		 else
		 close(sock);
	 }
	 return retval;
}

unsigned int w5100::send(unsigned char sock , const unsigned char * buf, unsigned int buflen)
{
	uint16_t ptr,offaddr,realaddr,txsize,timeout;

	if (buflen <= 0 || sock != 0) return 0;

//	#if _DEBUG_MODE
//	printf("Send Size: %d\n",buflen);
//	#endif

	// Make sure the TX Free Size Register is available
	txsize=SPI_Read(SO_TX_FSR);
	txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));

//	#if _DEBUG_MODE
//	printf("TX Free Size: %d\n",txsize);
//	#endif

	timeout=0;
	while (txsize < buflen) {
		_delay_ms(1);

		txsize=SPI_Read(SO_TX_FSR);
		txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));

		// Timeout for approx 1000 ms
		if (timeout++ > 1000) {
//			#if _DEBUG_MODE
//			printf("TX Free Size Error!\n");
//			#endif
			// Disconnect the connection
			disconnect(sock);
			return 0;
		}
	}

	// Read the Tx Write Pointer
	ptr = SPI_Read(S0_TX_WR);
	offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_TX_WR + 1));
//	#if _DEBUG_MODE
//	printf("TX Buffer: %x\n",offaddr);
//	#endif

	while(buflen) {
		buflen--;
		// Calculate the real W5100 physical Tx Buffer Address
		realaddr = TXBUFADDR + (offaddr & TX_BUF_MASK);

		// Copy the application data to the W5100 Tx Buffer
		SPI_Write(realaddr,*buf);
		offaddr++;
		buf++;
	}

	// Increase the S0_TX_WR value, so it point to the next transmit
	SPI_Write(S0_TX_WR,(offaddr & 0xFF00) >> 8 );
	SPI_Write(S0_TX_WR + 1,(offaddr & 0x00FF));

	// Now Send the SEND command
	SPI_Write(S0_CR,CR_SEND);

	// Wait for Sending Process
	while(SPI_Read(S0_CR));

	return 1;
}

unsigned int w5100::recv(unsigned char sock,unsigned char * buf, unsigned int buflen)
{
	uint16_t ptr,offaddr,realaddr;

	if (buflen <= 0 || sock != 0) return 1;

	// If the request size > MAX_BUF,just truncate it
	if (buflen > MAX_BUF)
	buflen=MAX_BUF - 2;

	// Read the Rx Read Pointer
	ptr = SPI_Read(S0_RX_RD);
	offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_RX_RD + 1));
//	#if _DEBUG_MODE
//	printf("RX Buffer: %x\n",offaddr);
//	#endif

	while(buflen) {
		buflen--;
		realaddr=RXBUFADDR + (offaddr & RX_BUF_MASK);
		*buf = SPI_Read(realaddr);
		offaddr++;
		buf++;
	}
	*buf='\0';        // String terminated character

	// Increase the S0_RX_RD value, so it point to the next receive
	SPI_Write(S0_RX_RD,(offaddr & 0xFF00) >> 8 );
	SPI_Write(S0_RX_RD + 1,(offaddr & 0x00FF));

	// Now Send the RECV command
	SPI_Write(S0_CR,CR_RECV);
	_delay_us(5);    // Wait for Receive Process

	return 1;
}

unsigned int w5100::recv_size(void)
{
	 return ((SPI_Read(S0_RX_RSR) & 0x00FF) << 8 ) + SPI_Read(S0_RX_RSR + 1);
}

