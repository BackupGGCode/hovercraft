/*
 *  joyMotors.c
 *  joyMotor
 *
 *  Created by Katherine Gunion on 24/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "joyMotors.h"
#include "../Joystick/joystick.h"
#include "../Motor/Motor.h"
#include "../common.h"
#include "../uart/uart.h"



unsigned char X = 0, Y = 0, Z = 0;



void
updateMotor(){
	
	packet_t packet;
	
	sample_joystick(&X,&Y,&Z);
	//updateVertically();
	
	_delay_ms(250);
	char test[50];
	int numbChar = sprintf(test,"Inside Motor X: %d, Y: %d, Z: %d\n\r", X,Y,Z);
	
	updateVertically();
	
	packet.x = (uint8_t) X;
	packet.y = (uint8_t) Y;
	radio_send(HOV_ADDRESS, (uint8_t*) &packet);
	
	uart_write((uint8_t*)test,  numbChar);
	


}


void 
updateVertically(){
	

	int intY = (int) Y;
	int len = 0;
	char toWrite[50];
	uint8_t newSpeed = 0;
	
	if(intY > 125){ //To go Backwards
		setMotorDirection(BACKWARD);
		newSpeed = (intY-125)*2;
		setMotorSpeed( newSpeed);
		len = sprintf(toWrite, "Motor Speed Is Backward %d\r\n", intY);
	}else{
		setMotorDirection(FORWARD);
		newSpeed = 256 - intY*2;
		setMotorSpeed(newSpeed);
		len = sprintf(toWrite, "Motor Speed Is Forward %d\r\n", newSpeed);
	}
	
	
	
	
	uart_write((uint8_t *)toWrite, len);
	_delay_ms(250);
	/*
		switch (vars) {
		case 0:
			setMotorSpeed(250);
			len = sprintf(toWrite, "Motor Speed Is Forward%d\r\n", 250);
			break;
		case 1:
			setMotorSpeed(230);
			len = sprintf(toWrite, "Motor Speed Is Forward %d\r\n", 230);	   
			break;
		case 2:
			setMotorSpeed(180);
			len = sprintf(toWrite, "Motor Speed Is Middle %d\r\n", 180);	
			break;
		case 3:
			setMotorSpeed(230); 
			len = sprintf(toWrite, "Motor Speed Is backwards %d\r\n", 240);
			break;
		case 4:
			setMotorSpeed(250);
			len = sprintf(toWrite, "Motor Speed Is backwards %d\r\n", 250);
			break;
		default:
			len = sprintf(toWrite,"Error in Switch\r\n");
		
			
		
	}//end of Switch
	*/
	//uart_write(toWrite, len);
		
	
}


