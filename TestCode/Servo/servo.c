/*
 *  servo.c
 *  
 *
 *  Created by Katherine Gunion on 24/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include "servo.h"

void
servoInit(void){

	TCCR1A = (0<<WGM10) | (1<<WGM11) | (1<<COM1B1) | (0<<COM1B0); 
	/*Set the IO pins for output for  OCB1*/ 
	DDRB = 0xFF;
	/* Set the TOP value to 2500 this should mean that the TCNT1 counter
	 should reset itself every 50 Hz at a clock of 1Mz for 8 Mhz, set to 20000(hopefully)*/
	ICR1 = 20000;   
	/* Set the low timings for the three output pins for ports A in OCR1A*/
	OCR1B = 1600; //corresponds to 8% duty cycle or 2ms pulse width
}


void servoDuty(int duty){

	OCR1B = duty;
	
}

int main(){
	servoInit();
	
	return 0;
}