/**
 *
 * @file motor.c
 *
 * @brief Implementation of all the functions related to the motors
 *
 * @n @n CSC 560
 *
 * @author Katherine Gunion
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include  "../common.h"
#include <util/delay.h>
#include "Motor.h"

/**
 *
 * Sets the motor ON.
 *
 */
void setMotorON() {
	PORTD |= (1<<PORTD7);
}


/**
 * Sets the motor OFF.
 */
void setMotorOFF() {
      OCR0A = 0;
}


/**
 * Sets the motor SPEED.
 *
 * @param duty The duty cycle.
 */
void setMotorSpeed(uint8_t duty)
{
	OCR0A = duty;
}

/**
 *
 * Sets the motor direction.
 *
 * @param d The direction (either BACKWARD or FORWARD).
 *
 */
void setMotorDirection(int d) {
    if (d == BACKWARD)
        PORTC |= _BV(PORTC1);
	else
		PORTC &= ~(_BV(PORTC1));
}

/**
 * Motor initialization function.
 */
void motor_init()
{
    // TODO Remember to get rid of the extra pins (D1, D4, D5)
    // DDRD |= (1<<PORTD1) | (1<<PORTD7) | (1<<PORTD4) | (1<<PORTD5);
    DDRD |= (1<<PORTD7);
    DDRC |= (1<<PORTC1);
    DDRB |= (1<<PORTB7);   /** Set the direction register for PWM signal **/

    /** setup timer/counter 0  for fast PWM   **/
    TCCR0A = (1<<WGM00) | (1<<WGM01) | (0<<COM0A0) | (1<<COM0A1);
    TCCR0B = (0<<WGM02) | (0<<CS02) | (0<<CS01) | (1<<CS00);
}

