/**
 * Sender Main.
 * 
 * @author: Marco Yuen
 * @copyright: Copyright 2009 Marco Yuen
 * 
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "radio/radio.h"
#include "uart/uart.h"
#include "common.h"

#define UART_PRINT_BUFFFER 100
#define RECEIVE_ADDRESS 0xBEEF
#define MAIN_ADDRESS    0xFEED

volatile uint8_t radio_buffer[PAYLOAD_BYTES];
volatile int sendFlag = 0;

int main(void)
{
    char uartBuffer[UART_PRINT_BUFFFER];
    short uartBufLen = 0;
    cli();
    
    NO_CLK_PRESCALE();
    uart_init();
    radio_init(RECEIVE_ADDRESS, RECEIVE_MODE);
    
    sei();
    
    DDRD = 0xff;
    
    uartBufLen = snprintf(uartBuffer, UART_PRINT_BUFFFER,  "The Sender\r\n");
    uart_write((uint8_t*)uartBuffer, uartBufLen);
    
    int i = 0;
    
    for(;;) {
        if (sendFlag) {
            PORTD ^= _BV(PORTD4);
            radio_send(MAIN_ADDRESS, (uint8_t *)"eat this MESSAGE\r\n");
            for (i = 0; i < 2; ++i) _delay_ms(250);
        }
    }
    
    return 0;   /* never reached */
}

ISR (INT4_vect)
{
    int i;
    
    // displayMesg("Int4\r\n");
    PORTD ^= _BV(PORTD4);
    
    for (i = 0; i < PAYLOAD_BYTES; i++)
    {
        radio_buffer[i] = radio_get_byte();
    }

    /* print out the radio packet on uart */
    uart_write((uint8_t*)radio_buffer, PAYLOAD_BYTES);
    
    /* setup the radio to receive another packet */
    // radio_set_receive();
    sendFlag = 1;
}

