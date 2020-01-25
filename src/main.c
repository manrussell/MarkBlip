/*
    Port Mapping

    PA0..7		8 potentiometers
    PB0-PB4		Control Panel Switches - ROWS
    PB5-PB7		ISP programming header
    PB7			DAC LDAC signal (load both DAC ports synchronously)
    PC0-PC7		DATA port for DAC
    PD0		    RxD (MIDI IN)
    PD1		    Power ON/MIDI LED
    PD2		    Select DAC port A or B
    PD3		    DAC Write line
    PD4-PD7		Control Panel Switches - COLUMNS

    Timers	

    Timer0		not used
    Timer1		Time counter: CK/400      --> TCNT1 
    Timer2		Sample timer: (CK/8) / 32 --> 36363.63636 Hz

*/

/*
AVR programming notes
    DDRD  = 0b00000001; 	// 1 is output
    PORTD = 0b00000001; 	// 1 is 5V
    
    PORTB is output data register, 
	PINB is input data register.
    
    to enable pull-ups on input.
    if DDRB = 0 (is input), then set high using PORTB
    then PORTB = ( 1u << 3 );
    
*/


#include <avr/io.h>
#include <util/delay.h>
#include <uart.h>
#include <adc.h>
#include <switch.h>
#include <dac.h>
#include <waveforms.h>

#define POWER_LED			( 1u << PD1 )
#define POWER_LED_ON( )		( PORTD |= POWER_LED )
#define POWER_LED_OFF( )	( PORTD &= ~POWER_LED )


void main (void)
{
	uint16_t switches = 0;
	int16_t soundData = 0;
	
    /* port D outputs */
    DDRD = POWER_LED;	
	
	POWER_LED_ON( );

    /* make PB0-PB4 ( not using 4 but needs to be input/floating ) -rows, input */
    DDRB = 0;	
	
	/*  PD3 is DAC W/R, PD2 is DAC A/B */
	DDRD |= ( 1u << PD3 ) | ( 1u << PD2 );

	/* port C is all outputs to DAC */
	DDRC |= 0xFF;
	
	USART_init( );
	adc_init( );	
    
    _delay_ms(1000); //ensure  power led on for 1 second
    
    while(1)
    {   
		//tx_all_adc_pot_data( );
		
		
		switches = get_switch_states( );
		outbin16( switches ); newline( );
		//_delay_ms(100);
		
		
		soundData = RampWaveform( soundData );
		//soundData++;
		
		if( switches & 0x0001)
		{
			soundData = 255;
		}
		
		writeToDac( soundData );
		outhex16( soundData );
		newline( );
		//_delay_us(1);
		
		
		
    }  
}


