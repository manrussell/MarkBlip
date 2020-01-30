
/*
author of init( ) from ...
https://binaryupdates.com/usart-in-avr-atmega32a-microcontroller/
some by Mark Russell

*/

#ifndef __UART_C__
#define __UART_C__

#include <avr/io.h>
#include "uart.h"

/* Max Baud rate seems to be 57600 */
#define BAUDRATE 57600 
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)


/* Initialize USART */
void USART_init(void)
{
  UBRRH = (uint8_t)(BAUD_PRESCALLER>>8);
  UBRRL = (uint8_t)(BAUD_PRESCALLER);
  UCSRB = (1<<RXEN)|(1<<TXEN);
  UCSRC = (1<<UCSZ0)|(1<<UCSZ1)|(1<<URSEL);
}

/* Function to receive byte/char */
unsigned char USART_receive(void)
{
  while(!(UCSRA & (1<<RXC)));
  return UDR;
}

/* Function to send byte/char */
void USART_send( unsigned char data)
{
  while(!(UCSRA & (1<<UDRE)));
  UDR = data;
}

/* Send string */
void USART_putstring(char* StringPtr)
{
  while(*StringPtr != 0x00)
  {
    USART_send(*StringPtr);
	StringPtr++;
  }
  
}


static void USART_send_binary( uint32_t data, uint8_t len )
{
	for(uint8_t cnt = 0; cnt<len; cnt++)
	{
		USART_send( ((data & ( 1 << cnt)) >> cnt) + 48 );
	}
}

void USART_outbin8(uint8_t d)
{
	USART_send_binary( d, 8 );
}

void USART_outbin16(uint16_t d)
{
	USART_send_binary( d, 16 );
}

void USART_outbin32(uint32_t d)
{
	USART_send_binary( d, 32 );
}



static void USART_send_hex(uint32_t v, uint8_t len)
{
	uint8_t 	i;
	uint32_t 	s;
	char		c;

	for (i = 0; i < len; i++)
	{
		s = ((len - i) - 1) << 2;
		c = (v >> s) & 15;
		if (c > 9) c += (65 - 48 - 10);
		USART_send(c + 48);
	}
}

void USART_outhex8(uint8_t d)
{
	USART_send_hex(d,2);
}

void USART_outhex16(uint16_t d)
{
	USART_send_hex(d,4);
}

void USART_outhex32(uint32_t d)
{
	USART_send_hex(d,8);
}

void USART_newline( void )
{
	USART_send('\r'); 
	USART_send('\n');
}

#endif /* __UART_C__ */