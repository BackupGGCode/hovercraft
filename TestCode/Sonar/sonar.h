 #ifndef __SONAR_H__
 #define __SONAR_H__
 

		#define SONAR_DDR               DDRC                                            
		#define SONAR_PORT                              PORTC
 
		#define SONAR_PORTNUM_ECHO              PORTC7
		#define SONAR_ECHO_MASK                 (_BV(7))                                        
		#define SONAR_PORTNUM_PULSE             PORTC6
		#define SONAR_PULSE_MASK                (_BV(6))                                        
		#define SET_RISING_EDGE()       (TCCR3B |= _BV(ICES3))                          
		#define SET_FALLING_EDGE()      (TCCR3B &= ~(_BV(ICES3)))                       
		#define IS_RISING_EDGE()        (TCCR3B & _BV(ICES3))                           
		#define IS_FALLING_EDGE()       ~(TCCR3B & _BV(ICES3))                          
		#define SET_IC_ENABLE()         (TIMSK3 |= _BV(ICIE3))                          
		#define CLEAR_IC_FLAG()         (TIFR3 |= _BV(ICF3))                            
		#define ENABLE_ECHO()           (SONAR_PORT |= SONAR_ECHO_MASK)                 
		#define DISABLE_ECHO()          (SONAR_PORT &= ~(SONAR_ECHO_MASK))              
		#define ENABLE_PULSE()          (SONAR_PORT |= SONAR_PULSE_MASK)                
		#define DISABLE_PULSE()         (SONAR_PORT &= ~(SONAR_PULSE_MASK))             
		#define US_PER_INCH                     147                                                                     

		void sonar_init();
		void trigger_sonar();

		uint8_t read_distance();
		int signal_recieved ();

#endif //__SONAR_H__
