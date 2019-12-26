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

void main(void)
{
    uint8_t switches = 0;
    
    // PB0-PB4		Control Panel Switches - ROWS
    // PD4-PD7		Control Panel Switches - COLUMNS
    
    /* set port D as output and turn D1 on*/
    /* pins 4,5,6,7 must be set low */
    /* flash led for 1 second */
    DDRD 	= ( 1u << 7 ) | ( 1u << 6 ) | ( 1u << 5 ) |( 1u << 4 ) | ( 1u << 1 );
    PORTD 	= ( 1u << 1 ); 
    
    /* make sure these are set low */
    PORTD 	&= ~( 1u << 4 );
    PORTD 	&= ~( 1u << 5 );
    PORTD 	&= ~( 1u << 6 );
    PORTD 	&= ~( 1u << 7 );
    
    _delay_ms(1000); //1 second delay
    
    
    /* make rows input with pull ups */
    /* pin << 4 is midi pin, ( 1u << 4 ) | */
    DDRB  = 0;
    // PORTB = ( 1u << 4 ) | ( 1u << 3 ) | ( 1u << 2 ) | ( 1u << 1 ) |( 1u << 0 );
    PORTB = ( 1u << 3 );
    
    
    
    /* switch one is connected to pin B3 and pin D7 */
    while(1)
    {        
        switches = PINB & ( 1u << 3); // PORTB is output data register, PIN is input data register
    
        if ( ( PINB & ( 1u << 3) ) |
             ( PINB & ( 1u << 2) ) |
             ( PINB & ( 1u << 1) ) |
             ( PINB & ( 1u << 0) )
        )
        //if ( switches )
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