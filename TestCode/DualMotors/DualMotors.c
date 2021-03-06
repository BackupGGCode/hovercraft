/**
 * @file DualMotors.c
 * The driver for interacting with two motors.
 *
 * @author Marco Yuen <marcoy@gmail.com>
 */

#include "DualMotors.h"

void 
motorInit(motor_t * motor)
{
    *(motor->pwmDDR)       |= (1 << motor->pwmPin);
    *(motor->directionDDR) |= (1 << motor->directionPin);
}

void 
pwmInit()
{
    // Setup timer/counter 0 for fast PWM
    TCCR0A = (1<<WGM00) | (1<<WGM01) | (0<<COM0A0) | (1<<COM0A1) | \
             (0<<COM0B0) | (1<<COM0B1);
    TCCR0B = (0<<WGM02) | (0<<CS02)  | (0<<CS01)   | (1<<CS00);
}

void
setMotorDuty(motor_t *motor, int duty)
{
    *(motor->topRegister) = duty;
}

void 
setMotorDirection(motor_t *motor, direction_t direction)
{
    if (direction == FORWARD)
        *(motor->directionPort) |= _BV(motor->directionPin);
    else
        *(motor->directionPort) &= ~(_BV(motor->directionPin));
}

