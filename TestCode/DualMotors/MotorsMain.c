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
    motor_t rightMotor = {
                           &PORTB, &DDRB, PORTB7, // PWM
                           &PORTC, &DDRC, PORTC1, // Direction
                           &OCR0A                 // Top value
                         };
    
    motor_t leftMotor = {
                          &PORTD, &DDRD, PORTD0, // PWM
                          &PORTB, &DDRB, PORTB0, // Direction
                          &OCR0B                 // Top value
                        };
    
    cli();
    NO_CLK_PRESCALE();
    motorInit(&leftMotor);
    motorInit(&rightMotor);
    pwmInit();
    sei();
    
    DDRD |= (1<<PORTD7);
    PORTD |= _BV(PORTD7);
    
    setMotorDuty(&leftMotor, 255);
    setMotorDuty(&rightMotor, 255);
    
    int i=0;
    for (i=0; i<16; ++i) _delay_ms(50);
    
    PORTD &= ~(_BV(PORTD7));
    
    setMotorDirection(&leftMotor, FORWARD);
    setMotorDirection(&rightMotor, FORWARD);
    
    for(;;){
        /* insert your main loop code here */
    }
    return 0;   /* never reached */
}
