/*
 *  servo.c
 *
 *
 *  Created by Katherine Gunion on 24/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include <avr/interrupt.h>
#include <util/delay.h>
#include "servo.h"


#define NO_PRESCALE() CLKPR=(1<<CLKPCE); \
                      CLKPR=0x00

void
servoInit(void){

	TCCR1B = (0<<CS10)|(1<<CS11)|(0<<CS12)|(1<<WGM13)|(1<<WGM12);

	TCCR1A = (0<<WGM10) | (1<<WGM11) | (1<<COM1B1) | (0<<COM1B0);
	/*Set the IO pins for output for  OCB1*/
	DDRB = 0xFF;
	/* Set the TOP value to 2500 this should mean that the TCNT1 counter
	 should reset itself every 50 Hz at a clock of 1Mz for 8 Mhz, set to 20000(hopefully)*/
	ICR1 = 20000;
	/* Set the low timings for the three output pins for ports A in OCR1A*/
	OCR1B = 800; //corresponds to 8% duty cycle or 2ms pulse width
}


int servoDuty(int duty){

	OCR1B = duty;
	return duty;
}

int main(){

	cli();
	NO_PRESCALE();
	servoInit();

	sei();
	
	int D = 0;
	D = servoDuty(600);
	
	for(;;)
		{
			
			
			_delay_ms(250);
			if(D > 2300)
				D = 600;
			D = servoDuty(D + 100);
		
		}

	return 0;
}
