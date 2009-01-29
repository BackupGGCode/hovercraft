/**
 * TX_RX Main.
 * 
 * @author: Marco Yuen
 * @copyright: Copyright 2009 Marco Yuen
 * 
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "radio/radio.h"
#include "uart/uart.h"
#include "common.h"

#define UART_PRINT_BUFFFER 100
#define RECEIVE_ADDRESS 0xBEEF
#define MAIN_ADDRESS    0xFEED

volatile uint8_t radio_buffer[PAYLOAD_BYTES];
uint8_t uartBuffer[UART_PRINT_BUFFFER];
short uartBufLen = 0;

void inline __attribute__((always_inline))
delay(uint8_t facter)
{
    int i = 0;
    for (i = 0; i < facter; ++i) _delay_ms(250);
}

void inline __attribute__((always_inline))
displayMesg(char * mesg)
{
    uartBufLen = snprintf((char *)uartBuffer, UART_PRINT_BUFFFER, mesg);
    uart_write(uartBuffer, uartBufLen);
}

int 
main(void)
{
    cli();
    
    NO_CLK_PRESCALE();
    uart_init();
    radio_init(MAIN_ADDRESS, RECEIVE_MODE);
    
    sei();
    
    DDRD = 0xff;
    PORTD = 0x00;
    
    // uartBufLen = snprintf(uartBuffer, UART_PRINT_BUFFFER,  "Tester\r\n");
    // uart_write((uint8_t*)uartBuffer, uartBufLen);
    
    // uartBufLen = snprintf(uartBuffer, UART_PRINT_BUFFFER, "Sending Message\r\n");
    
    /*
     // Send 10 messages then switch to RX
     int i = 0;
     for (i = 0; i < 10; ++i) {
        PORTD ^= _BV(PORTD4);
        uart_write((uint8_t *)uartBuffer, uartBufLen);
        radio_send(RECEIVE_ADDRESS, (uint8_t *)"From MAIN: Message");
        delay();
     }
     */

    int i = 0;
    for (i = 0; i < 10; i++) {
        radio_send(RECEIVE_ADDRESS, (uint8_t *)"Main: the message\r\n");
        delay(2);
    }
    
    uartBufLen = snprintf((char *)uartBuffer, UART_PRINT_BUFFFER, "Switching to RX mode\r\n");
    uart_write(uartBuffer, uartBufLen);
    
    // delay(4);
    radio_set_receive();
    radio_set_receive();
    
    // Wait forever.
    for (;;) {
        PORTD ^= _BV(PORTD7);
        delay(2);
        // radio_set_receive();
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
    radio_set_receive();
}
