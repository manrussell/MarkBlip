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

#define POWER_LED			( 1u << 1 )
#define POWER_LED_ON( )		( PORTD |= POWER_LED )
#define POWER_LED_OFF( )	( PORTD &= ~POWER_LED )


#define COLUMN_ONE		( 1u << 7 )
#define COLUMN_TWO		( 1u << 6 )
#define COLUMN_THREE	( 1u << 5 )
#define COLUMN_FOUR		( 1u << 4 )

#define SET_COLUMN_ONE_OUTPUT( )	( DDRD 	|= COLUMN_ONE  )
#define SET_COLUMN_TWO_OUTPUT( )	( DDRD 	|= COLUMN_TWO  )
#define SET_COLUMN_THREE_OUTPUT( )	( DDRD 	|= COLUMN_THREE)
#define SET_COLUMN_FOUR_OUTPUT( )	( DDRD 	|= COLUMN_FOUR )
        
#define SET_COLUMN_ONE_LOW( )		( PORTD &= ~(COLUMN_ONE)  )
#define SET_COLUMN_TWO_LOW( )		( PORTD &= ~(COLUMN_TWO)  )
#define SET_COLUMN_THREE_LOW( )		( PORTD &= ~(COLUMN_THREE))
#define SET_COLUMN_FOUR_LOW( )		( PORTD &= ~(COLUMN_FOUR) )

/* to disable set pin as input. */
#define DISABLE_COLUMN_ONE( )		( DDRD &= ~(COLUMN_ONE)  )
#define DISABLE_COLUMN_TWO_LOW( )	( DDRD &= ~(COLUMN_TWO)  )
#define DISABLE_COLUMN_THREE_LOW( )	( DDRD &= ~(COLUMN_THREE))
#define DISABLE_COLUMN_FOUR_LOW( )	( DDRD &= ~(COLUMN_FOUR) )


#define ROW_ONE			( 1u << 3 )
#define ROW_TWO			( 1u << 2 )
#define ROW_THREE		( 1u << 1 )
#define ROW_FOUR		( 1u << 0 )
/* pin << 4 is midi pin, ( 1u << 4 ) | */

#define SET_ROW_ONE_INPUT( )		( DDRB &= ~(ROW_ONE)  )
#define SET_ROW_TWO_INPUT( )		( DDRB &= ~(ROW_TWO)  )
#define SET_ROW_THREE_INPUT( )		( DDRB &= ~(ROW_THREE))
#define SET_ROW_FOUR_INPUT( )		( DDRB &= ~(ROW_FOUR) )

#define SET_ROW_ONE_PULLUP( )		( PORTB |= ROW_ONE  )
#define SET_ROW_TWO_PULLUP( )		( PORTB |= ROW_TWO  )
#define SET_ROW_THREE_PULLUP( )		( PORTB |= ROW_THREE)
#define SET_ROW_FOUR_PULLUP( )		( PORTB |= ROW_FOUR )

#define READ_ROW_ONE( )		( PINB & ROW_ONE )
#define READ_ROW_TWO( )		( PINB & ROW_TWO  )
#define READ_ROW_THREE( )	( PINB & ROW_THREE)
#define READ_ROW_FOUR( )	( PINB & ROW_FOUR )


void main(void)
{
    uint16_t adcVal = 0;
	char buffer[5];
	
	USART_init( );
    
    /* port D outputs */
    DDRD 	= COLUMN_ONE | COLUMN_TWO | COLUMN_THREE | COLUMN_FOUR | POWER_LED;
    
    /* make sure these are set low, */
	SET_COLUMN_ONE_LOW( );	
	SET_COLUMN_TWO_LOW( );	
	SET_COLUMN_THREE_LOW( );
	SET_COLUMN_FOUR_LOW( );	

	POWER_LED_ON( );	
    
    _delay_ms(1000); //ensure  power led on for 1 second
    
    
    /* make rows input*/
    DDRB  = 0;
	
	SET_ROW_ONE_PULLUP( );
	SET_ROW_TWO_PULLUP( );	
	SET_ROW_THREE_PULLUP( );	
    SET_ROW_FOUR_PULLUP( );	
	
	adc_init( );
    
    
    /* switch one is connected to pin B3 and pin D7 */
    while(1)
    {   
		adcVal = read_adc(0);
		itoa( adcVal, buffer, 10);
		
		USART_putstring( buffer );
		USART_send('\r');
		USART_send('\n');
    }  
}

