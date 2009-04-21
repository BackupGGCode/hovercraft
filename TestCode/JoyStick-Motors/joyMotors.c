/**
 * @file joyMotors.c
 *
 * @brief Communicates with the hovercrafts about the joystick positions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "joyMotors.h"
#include "../Joystick/joystick.h"
#include "../Motor/Motor.h"
#include "../common.h"
#include "../uart/uart.h"

/** The X value of the joystick. */
unsigned char X = 0;

/** The Y value of the joystick. */
unsigned char Y = 0;

/** The Z value of the joystick. */
unsigned char Z = 0;

/**
 * Samples the joystick and sends out the values to the hovercraft.
 * The hovercraft will take the values from the radio packets and update
 * the servo direction and motor speed.
 */
void
updateMotor(){
	
	packet_t packet;
	
	sample_joystick(&X,&Y,&Z);
    
	_delay_ms(250);
	char test[50];
	int numbChar = sprintf(test,"Inside Motor X: %d, Y: %d, Z: %d\n\r", X,Y,Z);
	
	packet.x = (uint8_t) X;
	packet.y = (uint8_t) Y;
	radio_send(HOV_ADDRESS, (uint8_t*) &packet);
	
	uart_write((uint8_t*)test,  numbChar);
}
