/**
* File: 2wayRadio.c
* Team Gongshow: Marco, Darren, Anthony, Kat

* Description
* Contains the hovercraft radio functionality.  The radio is initially set to receive mode, then switches
* to send mode when a message needs to be sent.  at the end of sending, the mode is switched back to receive.

* During receive mode, we wait for intrupts to fire to process incoming packets.
* During send mode, interrupts are disabled, NO packets can be received at this time
**/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "radio/radio.h"
#include "uart/uart.h"
#include "2wayRadio.h"
#include "common.h"

#define RECEIVE_ADDRESS  0xBEEF


volatile uint8_t radio_buffer[PAYLOAD_BYTES];

// Function declairations:
void sendMessage(char* message);

int main(int argc, char**argv)
{
    DISABLE_INTERRUPTS();
    
    CLOCK_8MHZ();

    DDRD |= _BV(PORTD5);
    
    radio_init(RECEIVE_ADDRESS, RECEIVE_MODE);
    
    uart_init();

    ENABLE_INTERRUPTS();
    
    uart_write((uint8_t*)"UART START\r\n", 13);

	// Generally the for loop, loops forever, but we want to test the sending every
	// 500 ms
    for(;;){
	
	_delay_ms(5000);
	sendMessage("ABCDEFGHIJKLMNOPQRSTUVWZYZ\n\r");
	}

    return 0;
}

/**
 * The interrupt handler used by the receiver to process an incomming packet.
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

/**
 * This function sends a packet.  Interrupts are temporarily disabled, the mode is changed, packet sent, mode changed back
 * and interrupts reenabled
 **/
void sendMessage(char* message){

	// Turn Local LED on so we know it's sending.
	PORTD ^= _BV(PORTD7);

    /* send a full payload (28 bytes) to the receiver */
    radio_send(RECEIVE_ADDRESS, (uint8_t*)message);

	// Put the radio back into receive mode:
	radio_set_receive();
	
	// Turn Local LED off so we know sending is done.
	PORTD ^= _BV(PORTD7);
}

