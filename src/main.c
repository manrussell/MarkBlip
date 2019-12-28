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
    
    PORTB is output data register, PINB is input data register.
    
    to enable pull-ups on input.
    if DDRB = 0 (is input), then set high using PORTB
    then PORTB = ( 1u << 3 );
    
    

*/


#include <avr/io.h>
#include <util/delay.h>
//#include "test.h"

#define POWER_LED		( 1u << 1 )
#define POWER_LED_ON	( 1u << 1 )

#define COLUMN_ONE		( 1u << 7 )
#define COLUMN_TWO		( 1u << 6 )
#define COLUMN_THREE	( 1u << 5 )
#define COLUMN_FOUR		( 1u << 4 )

#define ROW_ONE			( 1u << 3 )
#define ROW_TWO			( 1u << 2 )
#define ROW_THREE		( 1u << 1 )
#define ROW_FOUR		( 1u << 0 )

    // PB0-PB4		Control Panel Switches - ROWS
    // PD4-PD7		Control Panel Switches - COLUMNS

void main(void)
{
    uint8_t switches = 0;
    
    /* port D outputs */
    /* flash led for 1 second */
    DDRD 	= COLUMN_ONE | COLUMN_TWO | COLUMN_THREE | COLUMN_FOUR | POWER_LED;
	
	PORTD 	= POWER_LED_ON; 
    
    /* make sure these are set low, */
    PORTD 	&= ~( 1u << 4 ); // COLUMN_ONE	
    PORTD 	&= ~( 1u << 5 ); // COLUMN_TWO	
    PORTD 	&= ~( 1u << 6 ); // COLUMN_THREE
    PORTD 	&= ~( 1u << 7 ); // COLUMN_FOUR	
    
    _delay_ms(1000); //ensure  power led on for 1 second
    
    
    /* make rows input*/
    /* pin << 4 is midi pin, ( 1u << 4 ) | */
    DDRB  = 0;
	
	/* apply pull-ups*/
    // PORTB = ( 1u << 4 ) | ( 1u << 3 ) | ( 1u << 2 ) | ( 1u << 1 ) |( 1u << 0 );
    PORTB = ROW_ONE; // << 3
    
    
    
    /* switch one is connected to pin B3 and pin D7 */
    while(1)
    {        
        switches = PINB & ( 1u << 3) ; // doing this makes switches 1,2,3,4
    
        /* can't do this as switches 1,2,3,4 are row 1, and if any column pulls down that line to ground then
		if ( ( PINB & ( 1u << 3) ) |
             ( PINB & ( 1u << 2) ) |
             ( PINB & ( 1u << 1) ) |
             ( PINB & ( 1u << 0) )
        )*/
        if ( switches )
        {
            // set power led on
            PORTD |= ( 1u << 1 );
            
        }
        else
        {
            // power led off
            PORTD &= ~(1u << 1);
            
        }   
    }  
}