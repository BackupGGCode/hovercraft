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
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdio.h>


#include "sonar.h"
#include "../common.h"
#include "../uart/uart.h"



/**     Initial timestamp when echo pin goes high.
 */
long startTimestamp;
 
/**     The next index to put time into.
 */
long nextIndex = 0;

/**     Array to store last 5 readings made by sonar.
 */
long readings[5] = {0, 0, 0, 0, 0};

/**     The ideal distance in clock cycles used to compare current
 *      sonar readings to.
 */
long idealDistance = 0;

/**     The variance allowed on each side of the idealDistance
 *      before sending control function.
 */
long idleVariance = 0;

/**     The maximum variance. Any value above this will result
 *      in the duty being 100% to attempt quick recovery of location.
 */
long maxVariance = 0;

ISR(TIMER3_CAPT_vect){
 	uart_write((uint8_t*)"innInt\r\n",8);
	//Disable interupts on pin 7 of port C. (suspend input capture) 
	TIMSK3 &= ~(1<<ICIE3);
	
	
	//change edge, if rising edge just capted, change to fallig edge.
	if ((TCCR3B & (1<<ICES3))  == 0){
		//capture next rising edge
		TCCR3B |= (1<<ICES3);

                 if (ICR3 - startTimestamp < idealDistance + idleVariance + maxVariance) {
                         //      copy value from input capture register
                         readings[nextIndex] = ICR3 - startTimestamp;
                         
                 } else {
                         //      copy value from input capture register
                         readings[nextIndex] = idealDistance + idleVariance + maxVariance;
                 }
                 nextIndex = (nextIndex + 1) % 5;
                 sonarDistanceToDuty((readings[0] + readings[1] + readings[2] + readings[3] + readings[4]) / 5);
	} else {
     //      copy value from input capture register
	 	startTimestamp = ICR3;
 
	//      capture next falling edge
       	TCCR3B &= ~(1<<ICES3);

	}
//      enable interrupts on pin 4 of port D (resume input capture)
	TIMSK3 |= (1<<ICIE3);
	
} 
		 
/**
 * The interupt hander for the overflow of timer 3
 *
 *This will enable the Trigger pin for the sonar, so that the sonar will fire.
 */
ISR(TIMER3_OVF_vect){
	
	PORTD = (_BV(PORTD5)|_BV(PORTD7));
	
	fireSonar();
	
	

	

	
}

		 
/*
 *Code to fire the sonar.
 * 
 */
void fireSonar(){
	//Enable the pin to fire
	PORTA |= (1<<PORTA2);
	uint16_t current = TCNT3;
	//wait
		while(current + SONAR_PULSE > TCNT3);
	//Disable pin to fire
	PORTA &= ~(1<<PORTA2);

}
void sonarInit(){

		//Enable interupt capture pin PC7
	DDRC &= ~(1<<PORTC7);

	// Enable output as PD2 
	DDRA |= (1<<PORTA4);
	
	
	//When TOIEn is set, that means on overflow, interrupt is fired.
	//When ICIEn is set, then on Input Caput, interrupt is fired
	TIMSK3  |= (1<<TOIE3) | (1<<ICIE3);
	TIFR3 |= (1<ICF3);
	//Set the clock source to prescaling by 8, and enable noise cancelling.
	TCCR3B |= (1<<CS30) | (1<<ICNC3);
	
	// Set input camput edge to riding edge
	
	TCCR3B |= (1<<ICES3);
}


 void sonarDistanceToDuty(long distance) {
     long long offset = distance - idealDistance;
     long long percentage = 0;
        
      //      Turn off all leds used for output
    PORTD &= ~(1<<PIND5) & ~(1<<PIND6) & ~(1<<PIND7);

	 
	if (abs(offset) <= idleVariance) {
			
                return;

        } else if (offset < 0) {
                //      distance too low
            
           PORTD |= (1<<PIND6);
               // percentage = abs(offset) - idleVariance;
               // float variancePer = 100.0 / maxVariance;
              //  percentage = variancePer *percentage;
        } else {
                  //  distance too high
               
           		  PORTD |= (1<<PIND7);
                percentage = offset + idleVariance;

                float variancePer = 100.0 / maxVariance;
                percentage = variancePer *percentage;
         }
		 char toWrite[50];
		int len = sprintf(toWrite, "dist, %ld", distance);
		uart_write((uint8_t*)toWrite, len);
	 PORTD |= (1<<PIND5);

	}
