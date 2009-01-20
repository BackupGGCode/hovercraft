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
#include <avr/interrupt.h>

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
    cli();         // Disable interrupts

    NO_PRESCALE(); // Turn off system clock prescaler so it can run at 8Mhz
    setupInterrupts();

    sei();         // Enable interrupts

    // Enable the pins for input and pull them up
    DDRB  = 0x00;
    PORTB = 0xff;
    DDRE  = 0x00;
    PORTE = 0xff;

    DDRD  = 0xff;  // Set LEDs port for output
    PORTD = 0xff;  // Turn on both LEDs

    for (;;);

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
