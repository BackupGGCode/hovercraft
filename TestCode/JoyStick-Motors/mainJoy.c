/*
 *  mainJoy.c
 *  
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

#define TIMEOUT_VAL 1000

// A hack to deal with a problem in the radio driver.
#define RADIO_SET_RECEIVE() do { \
                                radio_set_receive(); \
                                radio_set_receive(); \
                            } while(0)

volatile uint8_t radio_buffer[PAYLOAD_BYTES];
volatile bool sendJoyStickPacket = true;

// UART buffer
static volatile char * printBuf[255];
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
				for(i = 0; i<4; ++i) _delay_ms(250);
			}
			sendJoyStickPacket = false;
            
            // After four control packets, wait for sonar information.
            RADIO_SET_RECEIVE();
		} else {
            _delay_ms(25);
		}
	}
	
	return 0;
}

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

