/**
 * Main.
 * 
 * @author: Katherine Gunion
 * @copyright: Copyright 2009 Katherine Gunion
 * 
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>
#include "common.h"

volatile uint8_t radio_buffer[PAYLOAD_BYTES];
char toPrint[50];

volatile uint8_t x = 0;
volatile uint8_t y = 0;

int main(void)
{
	
	cli();
	uart_init();
	motor_init();
	radio_init(HOV_ADDRESS, RECEIVE_MODE);
	
	sei();
	setMotorON();
    /* insert your hardware initialization here */
    for(;;){
        
		if( y < MIDDLE_Y){
			setMotorDirection(FORWARD);
			setMotorSpeed(255-(y*3));
		}else{
			setMotorDirection(BACKWARD);
			setMotorSpeed(y + 75);
		}
			
		/* insert your main loop code here */
		_delay_ms(500);
    }
    return 0;   /* never reached */
}

ISR (INT4_vect)
{
    int i;
    
	packet_t* packet;
	
    // displayMesg("Int4\r\n");
    PORTD ^= _BV(PORTD4);
    
    for (i = 0; i < PAYLOAD_BYTES; i++)
    {
        radio_buffer[i] = radio_get_byte();
    }
	
	packet = (packet_t*) radio_buffer;
    /* print out the radio packet on uart */
	
	int len = sprintf(toPrint, "X %d, Y %d ", packet->x, packet->y);
	
	x = packet->x;
	y = packet->y;
	
    uart_write((uint8_t*)toPrint, len);
    
    /* setup the radio to receive another packet */
    radio_set_receive();
}

