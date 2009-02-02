/*
 *  mainJoy.c
 *  
 *
 *  Created by Katherine Gunion on 24/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include <stdio.h>
#include <util/delay.h>
#include "joyMotors.h"
#include "../uart/uart.h"
#include "../Joystick/joystick.h"
#include "../Motor/Motor.h"
#include "../common.h"

volatile uint8_t radio_buffer[PAYLOAD_BYTES];
volatile int packetReceived = 0;
volatile char * printBuf[255];

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
	packetReceived = 0;
	
	for(;;) {
		
		if (!packetReceived)  {
			for (j = 0; j < 4; ++j) {
				updateMotor();
				for(i = 0; i<4; ++i) _delay_ms(250);
			}
			packetReceived = 1;
		} else {
			radio_set_receive();
			radio_set_receive();
			_delay_ms(250);
		}
	}
	
	return 0;
}

ISR(INT4_vect) 
{
    int i;
    
    // displayMesg("Int4\r\n");
    PORTD ^= _BV(PORTD4);
    
    for (i = 0; i < PAYLOAD_BYTES; i++)
    {
        radio_buffer[i] = radio_get_byte();
    }
	
    /* print out the radio packet on uart */
    int sonarDist = ((packet_t *) radio_buffer)->x;
	int len = sprintf((char *)printBuf, "Sonar: %d\r\n", sonarDist);
	uart_write((uint8_t*)printBuf, len);
    
    /* setup the radio to receive another packet */
    radio_set_receive();
	
	packetReceived = 0;
}

