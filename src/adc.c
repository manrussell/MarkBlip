
/*
code lifted from
https://binaryupdates.com/adc-in-avr-atmega32a-microcontroller/
*/

#ifndef __ADC_C__
#define __ADC_C__

#include <avr/io.h>
#include <uart.h>


void adc_init(void)
{
  ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // 16Mhz/128 = 125Khz the ADC reference clock
  ADMUX |= (1<<REFS0);              // Voltage reference from Avcc (5v)
  ADCSRA |= (1<<ADEN);              // Turn on ADC
  ADCSRA |= (1<<ADSC);              // Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}

uint16_t read_adc(uint8_t channel)
{
  ADMUX &= 0xF0;              // Clear the older channel that was read
  ADMUX |= channel;              // Defines the new ADC channel to be read
  ADCSRA |= (1<<ADSC);              // Starts a new conversion
  while(ADCSRA & (1<<ADSC));            // Wait until the conversion is done
  return ADCW;                // Returns the ADC value of the chosen channel
}

void tx_all_adc_pot_data( void )
{
    uint16_t adcVal = 0;
	char buffer[5]; // upto 1023 + newline
	
	/* start with 8 so it prints out to screen to match the hw infront of you */
	for( uint8_t channel = 8; channel > 0; --channel )
	{
		adcVal = read_adc( channel-1 );
		itoa( adcVal, buffer, 10);
	
		USART_putstring( buffer );
		USART_send('\t');
	}
	
	USART_send('\r');
	USART_send('\n');

}

#endif /* __ADC_C__ */