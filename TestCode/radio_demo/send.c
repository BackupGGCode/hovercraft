/**
 * @file   send.c
 * @author Justin Tanner
 * @date   Mon Sep 15 20:44:50 2008
 * 
 * @brief  Radio demonstration code
 *         This is the sending side of the radio demonstration
 *         Sends a test string to the receiver
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "radio/radio.h"
#include "uart/uart.h"
#include "common.h"

#define RECEIVE_ADDRESS 0xBEEF
#define SEND_ADDRESS    0xFEED

volatile uint8_t radio_buf[PAYLOAD_BYTES];

int main(int argc, char**argv)
{
    DISABLE_INTERRUPTS();
    
    CLOCK_8MHZ();

    DDRD |= _BV(PORTD5);
    
    radio_init(SEND_ADDRESS, TRANSMIT_MODE);
    
    ENABLE_INTERRUPTS();

    for(;;)
    {
        PORTD ^= _BV(PORTD5);
        
        /* send a full payload (28 bytes) to the receiver */
        radio_send(RECEIVE_ADDRESS, (uint8_t*)"ABCDEFGHIJKLMNOPQRSTUVWZYZ\n\r");
        
        _delay_ms(1000);
    }
    
    return 0;
}
