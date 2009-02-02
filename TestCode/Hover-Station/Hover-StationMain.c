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
#include "Servo/servo.h"
#include "Sonar/sonar.h"

// A hack to deal with a problem in the radio driver.
#define RADIO_SET_RECEIVE() do { \
                                radio_set_receive(); \
                                radio_set_receive(); \
                            } while(0)

#define DELAY_VAL 500
#define TIMEOUT_VAL 5

volatile uint8_t radio_buffer[PAYLOAD_BYTES];
volatile uint8_t x = 0;
volatile uint8_t y = 0;
volatile uint8_t son = 0;
volatile char toPrint[50];
volatile int len;
volatile int timeoutCounter = 0;

int 
main(void)
{
	
	cli();
    NO_CLK_PRESCALE();
	uart_init();
	motor_init();
	radio_init(HOV_ADDRESS, RECEIVE_MODE);
	servoInit();
	sonar_init();
	sei();
    
	setMotorON();
	
    for(;;) {
		
		son = read_distance();
		int i = 0;

        // 2-way communication
		if (TIMEOUT_VAL <= timeoutCounter) {
			len = sprintf((char *)toPrint, "Sending: %d\r\n", son);
			uart_write((uint8_t *)toPrint, len);
			packet_t p = {son, son};
			for(i = 0; i < 2; ++i) {
				radio_send(BASE_ADDRESS, (uint8_t *)&p);
				_delay_ms(25);
			}
			timeoutCounter = 0;
            RADIO_SET_RECEIVE();
		}
        
        // Motor control with sonar override
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
        
        // Servo control
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
        
		_delay_ms(DELAY_VAL);
		trigger_sonar();
		++timeoutCounter;
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
	
	int len = sprintf((char *)toPrint, "X %d, Y %d\r\n", packet->x, packet->y);
	
	x = packet->x;
	y = packet->y;
	
    uart_write((uint8_t*)toPrint, len);
    timeoutCounter = 0;

    /* setup the radio to receive another packet */
    radio_set_receive();
}

