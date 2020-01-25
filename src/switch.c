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





#endif /* __SWITCH_C__ */