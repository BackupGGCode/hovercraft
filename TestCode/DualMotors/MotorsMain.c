/**
 * Main.
 * 
 * @author: Marco Yuen
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "DualMotors.h"
#include "common.h"

int main(void)
{
    /* insert your hardware initialization here */
    motor_t leftMotor = {
                          &PORTC, &DDRC, PORTC1, // PWM
                          &PORTB, &DDRB, PORTB7  // Direction
                        };
    
    motor_t rightMotor = {
                           &PORTC, &DDRC, PORTC1,
                           &PORTB, &DDRB, PORTB7
                         };
    
    cli();
    NO_CLK_PRESCALE();
    motorInit(&leftMotor);
    motorInit(&rightMotor);
    pwmInit();
    sei();
    
    for(;;){
        /* insert your main loop code here */
    }
    return 0;   /* never reached */
}
