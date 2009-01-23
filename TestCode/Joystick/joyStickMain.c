/*
 * =====================================================================================
 *
 *       Filename:  jobLedMain.c
 *
 *    Description:  Using joystick to toggle the LEDs
 *
 *         Author:  Marco Yuen <marcoy@gmail.com>
 *
 * =====================================================================================
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "joystick.h"
#include "../2wayRadio/uart/uart.h"
#define NO_PRESCALE() CLKPR=(1<<CLKPCE); \
                      CLKPR=0x00

inline void __attribute__((always_inline))
setupInterrupts(void)
{
    // INT4 - Right
    // First set to detect any logical change on INT4
    EICRB &= ~(_BV(ISC41));
    EICRB |= _BV(ISC40);
    // Enable INT4
    EIMSK |= _BV(INT4);

    // INT5 - Down
    // First set to detect any logical change on INT5
    EICRB &= ~(_BV(ISC51));
    EICRB |= _BV(ISC50);
    // Enable INT5
    EIMSK |= _BV(INT5);

    // PCINT7 - UP
    PCICR   = 0x01; // Enable Pin Change interrupt
    PCMSK0 |= _BV(PCINT7);
    PCMSK0 |= _BV(PCINT6);
}

int
main(int argc, char *argv[])
{
	unsigned char X = 0, Y = 0, Z = 0;

    cli();
	         // Disable interrupts

    NO_PRESCALE(); // Turn off system clock prescaler so it can run at 8Mhz
    //setupInterrupts();
	uart_init();
	joystick_init();
    sei();         // Enable interrupts


    DDRD  = 0xff;  // Set LEDs port for output
    PORTD = 0xff;  // Turn on both LEDs

    for (;;){

	sample_joystick(&X, &Y, &Z);

	char test[50];
	int numbChar = sprintf(test,"X: %d, Y: %d, Z: %d\n\r", X,Y,Z);

	uart_write((uint8_t*)"hi\r\n", 4);

	_delay_ms(1000);

	uart_write((uint8_t*)test,  numbChar);
	
	_delay_ms(1000);
	
	
	}

    return 0;
}

ISR(INT4_vect)
{
    PORTD = (_BV(PORTD5)|_BV(PORTD7));
}

ISR(INT5_vect)
{
    PORTD = _BV(PORTD5);
}

ISR(PCINT0_vect)
{
    if (bit_is_set(PORTB, PINB6)) {
        PORTD = (_BV(PORTD4)|_BV(PORTD6));
    } else {
        PORTD = _BV(PORTD7);
    }
}
