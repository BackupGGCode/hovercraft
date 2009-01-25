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

int
main(void){
	
	Disable_Interrupt();
	
	CLOCK_8MHZ(); 
	motor_init();
	joystick_init();
	uart_init();
	
	Enable_Interrupt();
	

	
	
	uart_write((uint8_t*)"UART.START\r\n", 12);
	
	for(;;){
	
		updateMotor();

	
	
	int i;
	for(i = 0; i<4;i++)
		_delay_ms(250);
	
	}
	
	return 0;
}

