#ifndef __SWITCH_H__
#define __SWITCH_H__

/* col 4 */
#define SWITCH_16 ( 0  )
#define SWITCH_12 ( 1  )
#define SWITCH_8  ( 2  )
#define SWITCH_4  ( 3  )

/* col 3 */
#define SWITCH_15 ( 4  )
#define SWITCH_11 ( 5  )
#define SWITCH_7  ( 6  )
#define SWITCH_3  ( 7  )

/* col 2 */
#define SWITCH_14 ( 8  )
#define SWITCH_10 ( 9  )
#define SWITCH_6  ( 10 )
#define SWITCH_2  ( 11 )

/* col 1 */
#define SWITCH_13 ( 12 )
#define SWITCH_9  ( 13 )
#define SWITCH_5  ( 14 )
#define SWITCH_1  ( 15 )

#define SWITCH_1_DOWN  ( 1 << SWITCH_1 )
#define SWITCH_2_DOWN  ( 1 << SWITCH_2 )
#define SWITCH_3_DOWN  ( 1 << SWITCH_3 )
#define SWITCH_4_DOWN  ( 1 << SWITCH_4 )
#define SWITCH_5_DOWN  ( 1 << SWITCH_5 )
#define SWITCH_6_DOWN  ( 1 << SWITCH_6 )
#define SWITCH_7_DOWN  ( 1 << SWITCH_7  )
#define SWITCH_8_DOWN  ( 1 << SWITCH_8  )
#define SWITCH_9_DOWN  ( 1 << SWITCH_9  )
#define SWITCH_10_DOWN ( 1 << SWITCH_10  )
#define SWITCH_11_DOWN ( 1 << SWITCH_11  )
#define SWITCH_12_DOWN ( 1 << SWITCH_12  )
#define SWITCH_13_DOWN ( 1 << SWITCH_13  )
#define SWITCH_14_DOWN ( 1 << SWITCH_14  )
#define SWITCH_15_DOWN ( 1 << SWITCH_15  )
#define SWITCH_16_DOWN ( 1 << SWITCH_16  )


#define SWITCH_DOWN( switch_state , switch_number) (  switch_state & ( 1U << switch_number) )

/* SWITCH_CHECK turns out is actually the same as switch down 
-- maybe check which gives better assembly??
*/
//#define SWITCH_CHECK( switch_state , switch_number) (( switch_state >> switch_number) & 1U)

/* if ( switchDown(switches, SWITCH_1) )  
these inline function did not work, left here for future consideration!

static inline int switchDown( uint16 switches, int switch_num  )
{
	return ( switches & ( 1 << switch_num) );
}

static inline int switchCheck( uint16 switches, int switch_num  )
{
	return ( (switches >> switch_num) & 1U );
}
*/



uint16_t get_switch_states( void );

#endif /* __SWITCH_H__ */