/**
 * @file DualMotors.h
 *
 * @author Marco Yuen <marcoy@gmail.com>
 *
 */

#ifndef __DUAL_MOTORS_H
#define __DUAL_MOTORS_H

#include <avr/io.h>

/**
 * A motor type.
 */
typedef struct
{
    /** The port at which the PWM signal is coming out of. */
    volatile uint8_t *pwmPort;
    
    /** The data direction register for PWM. */
    volatile uint8_t *pwmDDR;
    
    /** The PWM pin. */
    uint8_t pwmPin;
    
    /** The port at which the direction of the motor can be controlled. */
    volatile uint8_t *directionPort;
    
    /** The data direction register for the direction pin. */
    volatile uint8_t *directionDDR;
    
    /** The direction pin. */
    uint8_t directionPin;
} motor_t;

/**
 * Initialization method for the motors.
 */
void motorInit(motor_t * motor);

/**
 * Initializes the Timer/Counter 0 for PWM.
 */
void pwmInit();

#endif

