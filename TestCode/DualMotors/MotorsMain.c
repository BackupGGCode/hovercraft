/**
 * Main.
 * 
 * @author: Marco Yuen
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "DualMotors.h"
#include "common.h"

int main(void)
{
    motor_t leftMotor = {
                          &PORTB, &DDRB, PORTB7, // PWM
                          &PORTC, &DDRC, PORTC1, // Direction
                          &OCR0A                 // Top value
                        };
    
    motor_t rightMotor = {
                           &PORTB, &DDRB, PORTB7, // PWM
                           &PORTC, &DDRC, PORTC1, // Direction
                           &OCR0A                 // Top value
                         };
    
    cli();
    NO_CLK_PRESCALE();
    motorInit(&leftMotor);
    motorInit(&rightMotor);
    pwmInit();
    sei();
    
    setMotorDuty(&leftMotor, 230);
    
    int i=0;
    for (i=0; i<16; ++i) _delay_ms(50);
    
    setMotorDirection(&leftMotor, FORWARD);
    setMotorDuty(&leftMotor, 230);
    
    for(;;){
        /* insert your main loop code here */
    }
    return 0;   /* never reached */
}
