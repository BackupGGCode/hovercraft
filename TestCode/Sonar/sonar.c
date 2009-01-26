/*
 *  sonar.c
 *  Sonar
 *
 *  Created by Katherine Gunion on 25/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 * SonarControl.c by Micheal Clark was used as a guide.
 *
 */

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>


#include "sonar.h"
#include "../common.h"
#include "../uart/uart.h"

/** Initial timestamp when echo pin goes high
 */


SIGNAL(TIMER3_CAPT_vect){

	//Disable interupts on pin 7 of port C. (suspend input capture) 
	TIMSK3 &= ~(1<<ICIE3);
	
	//change edge, if rising edge just capted, change to fallig edge.
	if ((TCCR3B & (1<<ICES3))  == 0){
		//capture next rising edge
		TCCR3B |= (1<<ICES3);
	}
	
} 
		 
/**
 * The interupt hander for the overflow of timer 3
 *
 *This will enable the Trigger pin for the sonar, so that the sonar will fire.
 */
SIGNAL(TIMER3_OVF_vect){
	
	PORTD	|= (1<<PIND5);
	
	fireSonar();
	
	PORTD &= ~(1<<PIND5);Turn off LED;
	
}

		 
/*
 *Code to fire the sonar.
 * 
 */
void fireSonar(){
	//Enable the pin to fire
	PORTD |= (1<<PORTD2);
	uint16_t current = TCNT3;
	//wait
	while(current + SONAR_PULSE > TCNT3);
	//Disable pin to fire
	PORTD &= ~(1<<PORTD2);

}
void sonarInit(){
	//Enable interupt captuer pin PD4
	DDRC &= ~(1<<PORTC7);
	
	// Enable output as PD2 
	DDRD |= (1<<PORTD2);
	
	
	//When TOIEn is set, that means on overflow, interrupt is fired.
	//When ICIEn is set, then on Input Caput, interrupt is fired
	TIMSK1 = |= (1<<TOIE3) | (1<<ICIE3);
		
	//Set the clock source to prescaling by 8, and enable noise cancelling.
	TCCR3B |= (1<<CS30) | (1<<ICNC3)
	
	// Set input camput edge to riding edge
	
	TCCR3B |= (1<<ICES3)
}