#ifndef __DAC_C__
#define __DAC_C__

#include <avr/io.h>

void writeToDac(int16_t data)
{
/*
	Write the 16-bit signed output of the DCA to the DAC.
	line 1092 from assembly file

    sbi		PORTD, 3			; Set WR high
    subi	r31, 128		    ; U2 --> PB
    cbi		PORTD, 2			; Select DAC port A
    out	    PORTC, r31	        ; output most significant byte
    cbi		PORTD, 3			; Pull WR low to load buffer A
    sbi		PORTD, 3			; Set WR high
    sbi		PORTD, 2			; Select DAC port B
    out	    PORTC, r30	        ; output least significant byte
    cbi		PORTD, 3			; Pull WR low to load buffer B
    sbi		PORTD, 3			; Set WR high again
*/   

    // Set WR high
    PORTD |= ( 1u << PD3 );
    
    // ignore subi	r31, 128		    ; U2 --> PB
    // what does this do? minus 128 from MSByte so it can be output as a byte 
    // data >> 
    
    // Select DAC port A
    PORTD &= ~( 1u << PD2 );
    
    // output most significant byte
    PORTC = (data >> 8) -128 ;     // do shift/subtraction here 
  
    // Pull WR low to load buffer A
    PORTD &= ~( 1u << PD3 );
    
    // Set WR high
    PORTD |= ( 1u << PD3 );
    
    // Select DAC port B
    PORTD |= ( 1u << PD2 );
    
    // output least significant byte
    PORTC = data & 0xFF;
    
    // Pull WR low to load buffer B
    PORTD &= ~( 1u << PD3 );
    
    // Set WR high again
    PORTD |= ( 1u << PD3 );

}

#endif /* __DAC_C__ */