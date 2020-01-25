#ifndef __WAVEFORMS_C__
#define __WAVEFORMS_C__

#include <avr/io.h>
//#include <util/delay.h>
//#include <dac.h>
#include "waveforms.h"


int16_t RampWaveform( int16_t sound )
{
	return ++sound;
}


int16_t TriangularWaveform8bit( int16_t sound )
{
	// triangular wave
	//https://stackoverflow.com/questions/1073606/is-there-a-one-line-function-that-generates-a-triangle-wave
	/* more of a 'v' wave
	-3  3
	-2  2
	-1  1
	0   0
	1   1
	2   2
	3   3
	*/
	// only go upto 255for 8 bit-ness
	//writeToDac ( abs((sound++ % 255) - 128) );
	
	
	return ( abs( (sound++ % 255) - 128));
}

int16_t TriangularWaveform16bit( int16_t sound )
{	
	// 16 bit-ness
	return ( abs( (sound++ % 65535) - 32767));
}

#endif /* __WAVEFORMS_C__ */