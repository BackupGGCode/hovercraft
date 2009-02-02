/**
 * Main.
 * 
 * @author: Katherine Gunion
 * @copyright: Copyright 2009 Katherine Gunion
 * 
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <stdlib.h>
#include "common.h"
#include "../Servo/servo.h"
#include "../Sonar/sonar.h"


volatile uint8_t radio_buffer[PAYLOAD_BYTES];
volatile uint8_t x = 0;
volatile uint8_t y = 0;
volatile uint8_t son = 0;
volatile int pktCounter = 0;
volatile char toPrint[50];
volatile int len;

int main(void)
{
	
	cli();
	uart_init();
	motor_init();
	radio_init(HOV_ADDRESS, RECEIVE_MODE);
	NO_CLK_PRESCALE();
	servoInit();
	sonar_init();
	
	sei();
	setMotorON();
	
    /* insert your hardware initialization here */
    for(;;){
		
		son = read_distance();
		int i = 0;

		if (pktCounter == 4) {
			len = sprintf(toPrint, "Sending sonar: %d\r\n", son);
			uart_write((uint8_t *)toPrint, len);
			packet_t p = {son, son};
			for(i = 0; i < 10; ++i) {
				radio_send(BASE_ADDRESS, (uint8_t *)&p);
				_delay_ms(250);
			}
			pktCounter = 0;
			radio_set_receive();
			radio_set_receive();
		}
		
	//	int len = sprintf(buff, "%d\r\n", son);
	//	uart_write((uint8_t*)buff, len);
		if (son < 12) {
			setMotorSpeed(0);
		} else {
				
			if( y < MIDDLE_Y){
				setMotorDirection(FORWARD);
				setMotorSpeed(255-(y*3));
			}else{
				setMotorDirection(BACKWARD);
				setMotorSpeed(y + 75);
			}
		}
			if (x >= 130 && x < 155) {
				servoDuty(1350);
			}
		
			else if (x >= 0 && x < 65) {
				servoDuty(570);
			} else if (x >= 202 && x <= 255) {
				servoDuty(2350);
			} else if (x >=65 && x < 130) {
				servoDuty(960);
			} else if (x >= 150 && x < 202) {
				servoDuty(1850);
			}
		
			
		/* insert your main loop code here */
		_delay_ms(500);
		trigger_sonar();
		
    }
	
    return 0;   /* never reached */
}

ISR (INT4_vect)
{
    int i;
    
	packet_t* packet;
	PORTD ^= _BV(PORTD4);
    
    for (i = 0; i < PAYLOAD_BYTES; i++)
    {
        radio_buffer[i] = radio_get_byte();
    }
	
	packet = (packet_t*) radio_buffer;
    /* print out the radio packet on uart */
	
	int len = sprintf(toPrint, "X %d, Y %d\r\n", packet->x, packet->y);
	
	x = packet->x;
	y = packet->y;
	
    uart_write((uint8_t*)toPrint, len);
    
    /* setup the radio to receive another packet */
    radio_set_receive();
	++pktCounter;
}

