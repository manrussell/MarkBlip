#ifndef __UART_H__
#define __UART_H__

/*
NOTE: one thing to be noted that we have to use 16 MHz external crystal with ATmega32A 
to function this code properly. And we have to burn fuse bits LFUSE: 0xFF and HFUSE:0x99 
after downloading hex file.
*/

void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

void outbin8 (uint8_t  d);
void outbin16(uint16_t d);
void outbin32(uint32_t d);

void outhex8 (uint8_t  d);
void outhex16(uint16_t d);
void outhex32(uint32_t d);

void newline( void );



#endif /* __UART_H__ */

