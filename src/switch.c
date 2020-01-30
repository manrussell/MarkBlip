#ifndef __SWITCH_C__
#define __SWITCH_C__

#include <avr/io.h>
#include "switch.h"

#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)
#define _NOPS_FOR_PIN_STABILITY( ) for( uint8_t i=0; i< 12; i++){ __asm__ __volatile__ ("nop"); }


/*
	PB0-PB4		Control Panel Switches - ROWS
	PD4-PD7		Control Panel Switches - COLUMNS
	
	when inputs have no pull-up they are just floating
	when columns are set to input they are also just floating
*/
/*
  ok so this returns all the 16 switches but ignores the midi channel switches, to do that 
  could return a 32_t? and make rows rowIndex < 5 
  not tested this though, also could make a seperate function to check that
  
  returned data:
  a bit will be '0' if it is 'down', and '1' if it is 'up'
  to check if a switch is 'up' or 'down' use the macros in switch.h
  
*/

uint16_t SWITCH_read_states( void )
{
	uint16_t switchState = 0xFFFF;
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
			
			// read pin, 'not' result so that switch 'up' position, is a '1'
			switchState &=  ~(( (PINB & ( 1 << rowIndex )) >> rowIndex ) << ( rowCnt + colCnt ));
			
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

/*
checked assembly for non/inverted ( )
non-inverted
2e0
3b8
= d8 = 216 instructions

inverted
3ba
48e
= d4 =212 instructions

-an extra 2 instructions for the // read pin
not sure where other 2 came from though,
its not the init of switchState

*/
/* no need for inverted */
#if 0
uint16_t SWITCH_read_states_inverted( void )
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
#endif


#endif /* __SWITCH_C__ */