/**
 * @file   receive.c
 * @author Justin Tanner
 * @date   Mon Sep 15 20:42:13 2008
 * 
 * @brief  Radio demonstration code
 *         This is the receiver only side of the radio demonstration
 *         Displays received packets via UART
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "radio/radio.h"
#include "uart/uart.h"
#include "common.h"

#define RECEIVE_ADDRESS  0xBEEF

volatile uint8_t radio_buffer[PAYLOAD_BYTES];

int main(int argc, char**argv)
{
    DISABLE_INTERRUPTS();
    
    CLOCK_8MHZ();

    DDRD |= _BV(PORTD5);
    
    radio_init(RECEIVE_ADDRESS, RECEIVE_MODE);
    
    uart_init();

    ENABLE_INTERRUPTS();
    
    uart_write((uint8_t*)"UART START\r\n", 13);

    for(;;);

    return 0;
}

/**
 * receive radio packet interrupt
 **/
ISR (INT4_vect)
{
    int i;

    PORTD ^= _BV(PORTD5);

    for (i = 0; i < PAYLOAD_BYTES; i++)
    {
        radio_buffer[i] = radio_get_byte();
    }

    /* print out the radio packet on uart */
    uart_write((uint8_t*)radio_buffer, PAYLOAD_BYTES);

    /* setup the radio to receive another packet */
    radio_set_receive();
}

