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

#define POWER_LED			( 1u << PD1 )
#define POWER_LED_ON( )		( PORTD |= POWER_LED )
#define POWER_LED_OFF( )	( PORTD &= ~POWER_LED )


#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)
#define _NOPS_FOR_PIN_STABILITY( ) for( uint8_t i=0; i< 12; i++){ __asm__ __volatile__ ("nop"); }


/* forward declarations */
void 	 tx_all_adc_pot_data( void );
uint16_t get_switch_states( void );

void main (void)
{
	uint16_t switches = 0;
	
    /* port D outputs */
    DDRD = POWER_LED;	
	
	POWER_LED_ON( );

    /* make PB0-PB4 ( not using 4 but needs to be input/floating ) -rows, input */
    DDRB = 0;	
	
	USART_init( );
	adc_init( );	
    
    _delay_ms(1000); //ensure  power led on for 1 second
    
    while(1)
    {   
		//tx_all_adc_pot_data( );
		switches = get_switch_states( );
		outbin16( switches );
		USART_send('\r');
		USART_send('\n');
		_delay_ms(100);
    }  
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

/*
	PB0-PB4		Control Panel Switches - ROWS
	PD4-PD7		Control Panel Switches - COLUMNS
	
	when inputs have no pull-up they are just floating
	when columns are set to input they are also just floating
*/
/*
  ok so this returns all the 16 switches but ignores the midi channel switches, to do that 
  could return a 32_t? and make rows rowIndex < 5 
  not tested this though
*/

uint16_t get_switch_states( void )
{
	uint16_t switchState = 0;
	uint8_t colCnt = 0;
	uint8_t rowCnt = 0;

	//iterate columns
	for( uint8_t colIndex = 4; colIndex < 8; colIndex++ )   
	{
		// set col to output and low
		DDRD  |=  ( 1 << colIndex );
		PORTD &= ~( 1 << colIndex );
		_NOPS_FOR_PIN_STABILITY( );
		
		rowCnt = 0;
		
		//iterate rows
		for( uint8_t rowIndex = 0;  rowIndex < 4; rowIndex++ )
		{			
			// set row with pullup
			PORTB |= ( 1 << rowIndex );
			_NOPS_FOR_PIN_STABILITY( );
			
			// read pin
			switchState |=  ( (PINB & ( 1 << rowIndex )) >> rowIndex ) << ( rowCnt + colCnt );
			
			// clear pullup
			PORTB &= ~( 1 << rowIndex );
			_NOPS_FOR_PIN_STABILITY( );
			
			rowCnt++;
			
		}
		
		//disable the column, make input
		DDRD &= ~( 1 << colIndex );
		_NOPS_FOR_PIN_STABILITY( );
		
		colCnt += 4;
	}
	
	return switchState;
}