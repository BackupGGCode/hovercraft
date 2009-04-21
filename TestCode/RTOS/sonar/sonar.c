/**
 *  @file sonar.c
 *  The sonar driver.
 *
 *  Created by Katherine Gunion on 25/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *  SonarControl.c by Micheal Clark was used as a guide.
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

/** The time at which the falling edge happened. */
static uint16_t time_falling = 0;

/** A flag to determine */
volatile int recieved = 0;

/** The UART buffer. */
char buff[50];

volatile uint8_t currentPulseMask = 0;

/**
 * Sonar initialization function.
 */
void sonar_init(){
//      initialize sonar
	DDRC |= FRONT_SONAR_PULSE_MASK | LEFT_SONAR_PULSE_MASK | RIGHT_SONAR_PULSE_MASK;                       
    TCCR3B &= ~(_BV(CS32) | _BV(CS30));     
    TCCR3B |= _BV(CS31);                            
    TCCR3B |= _BV(ICNC3);                           
//      enable timer and global interrupts
    SET_RISING_EDGE();
    CLEAR_IC_FLAG();
    SET_IC_ENABLE();
    ENABLE_ECHO();
    sei();
	int i=0;
    for(i=0; i < 63; ++i) _delay_ms(40);

}

/**
 * Sends pulses out via the sonar.
 */
void trigger_sonar(sonar_t sonar) {
    switch (sonar) {
        case FRONT:
            ENABLE_PULSE(FRONT_SONAR_PULSE_MASK);
            currentPulseMask = FRONT_SONAR_PULSE_MASK;
            break;
        case RIGHT:
            ENABLE_PULSE(RIGHT_SONAR_PULSE_MASK);
            currentPulseMask = RIGHT_SONAR_PULSE_MASK;
            break;
        case LEFT:
            ENABLE_PULSE(LEFT_SONAR_PULSE_MASK);
            currentPulseMask = LEFT_SONAR_PULSE_MASK;
            break;
    }
    
	SET_RISING_EDGE();
	CLEAR_IC_FLAG();
	SET_IC_ENABLE();
}

/**
 * Calculates the distance based on the echo.
 *
 * @return The distance in inches.
 */
uint8_t read_distance() {
	
	//sprintf(buff, "Radio init time_falling:%d", time_falling);
	//uart_write((uint8_t*)buff, strlen(buff));
	
	return (time_falling/US_PER_INCH);      
}

/**
 * Interrupt service routine for the sonar.
 */
ISR(TIMER3_CAPT_vect) {
	
	DISABLE_PULSE(currentPulseMask);                                        
	recieved = 1;
	
	if (IS_RISING_EDGE()) {
		TCNT3=0;
		SET_FALLING_EDGE();
		CLEAR_IC_FLAG();
	} else {
		time_falling = ICR3;                    
		SET_RISING_EDGE();
		CLEAR_IC_FLAG();
/*		int len = sprintf(buff, "Sonar Reads: %d\r\n", read_distance());
		uart_write((uint8_t*)buff, len);
        trigger_sonar();
*/	}

}

/**
 * Returns whether there was an echo or not.
 *
 * @return 1 or 0 depending on whether there was an echo.
 */
int signal_recieved() {
	int x = recieved;
	recieved = 0;
	return x;
}
