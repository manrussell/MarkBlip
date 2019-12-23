#include <avr/io.h>

void main(void)
{
	//DDRD 	= 0b00000001;
	//PORTD 	= 0b00000001;
	
	DDRD 	= ( 1u << 1 );
	PORTD 	= ( 1u << 1 ); /* led on D1 not D0 */
	
	while(1);
	
	
}