
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
#endif /* __UART_C__ */