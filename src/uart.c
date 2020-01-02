
/*
code author
https://binaryupdates.com/usart-in-avr-atmega32a-microcontroller/
*/

#ifndef __UART_C__
#define __UART_C__

#include <avr/io.h>

#define BAUDRATE 9600
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


void USART_send_binary( uint32_t data, uint8_t len )
{
	for(uint8_t cnt = 0; cnt<len; cnt++)
	{
		// uint8_t data = ((data & ( 1 << cnt)) >> cnt) + 48;
		// USART_send( data );
		USART_send( ((data & ( 1 << cnt)) >> cnt) + 48 );
	}
}

void outbin8(uint8_t d)
{
	USART_send_binary( d, 8 );
}

void outbin16(uint16_t d)
{
	USART_send_binary( d, 16 );
}

void outbin32(uint32_t d)
{
	USART_send_binary( d, 32 );
}



void USART_send_hex(uint32_t v, uint8_t len)
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

void outhex8(uint8_t d)
{
	USART_send_hex(d,2);
}

void outhex16(uint16_t d)
{
	USART_send_hex(d,4);
}

void outhex32(uint32_t d)
{
	USART_send_hex(d,8);
}


#endif /* __UART_C__ */