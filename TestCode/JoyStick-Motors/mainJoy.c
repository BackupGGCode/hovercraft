/**
 *  @file mainJoy.c
 *  @brief The driver for base station.
 *
 *  Created by Katherine Gunion on 24/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
#include "joyMotors.h"
#include "../uart/uart.h"
#include "../Joystick/joystick.h"
#include "../Motor/Motor.h"
#include "../common.h"

/**
 * The timeout value.
 */
#define TIMEOUT_VAL 1000

/**
 * A macro to handle a bug in the radio driver code. The radio_set_receive()
 * needs to call twice in order for it to successfully switch to receive mode.
 */
#define RADIO_SET_RECEIVE() do { \
                                radio_set_receive(); \
                                radio_set_receive(); \
                            } while(0)

/** The radio packet buffer. */
volatile uint8_t radio_buffer[PAYLOAD_BYTES];

/** The flag to deteremine when to listen for sonar information. */
volatile bool sendJoyStickPacket = true;

/** The share buffer for UART. */
static volatile char * printBuf[255];

/** The character length in the buffer. */
volatile int bufLen = 0;

int
main(void){
	
	Disable_Interrupt();
	
	CLOCK_8MHZ(); 
	motor_init();
	joystick_init();
	radio_init(BASE_ADDRESS, RECEIVE_MODE);
	uart_init();
	
	Enable_Interrupt();
	
	uart_write((uint8_t*)"UART.START\r\n", 12);
	int i = 0, j = 0;
	
	for(;;) {
		if (sendJoyStickPacket)  {
			for (j = 0; j < 4; ++j) {
                updateMotor();
                for(i = 0; i<1; ++i) _delay_ms(200);
			}
			sendJoyStickPacket = false;
            
            RADIO_SET_RECEIVE();
		} 
        
        else {
            _delay_ms(25);
		}
	}
	
	return 0;
}

/**
 * Interrupt handler for the radio.
 */
ISR(INT4_vect) 
{
    int i, sonarDist;
    
    PORTD ^= _BV(PORTD4);
    
    for (i = 0; i < PAYLOAD_BYTES; i++)
    {
        radio_buffer[i] = radio_get_byte();
    }
	
    /* print out the radio packet on uart */
    sonarDist = ((packet_t *) radio_buffer)->x;
	bufLen = sprintf((char *)printBuf, "Sonar: %d\r\n", sonarDist);
	uart_write((uint8_t*)printBuf, bufLen);
    
	sendJoyStickPacket = true;
    
    /* setup the radio to receive another packet */
    radio_set_receive();
}

