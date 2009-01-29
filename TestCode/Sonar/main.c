/*
 *  main.c
 *  Sonar
 *
 *  Created by Katherine Gunion on 25/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */
 
#include "../common.h"
#include "../uart/uart.h"
#include "sonar.h"
#include <avr/io.h>
#include <avr/interrupt.h>
 int	
 main(void){
		cli();
		CLOCK_8MHZ();
		DDRD  = 0xff;
		PORTD = 0xff;  // Turn on both LEDs
			uart_init();
		sonarInit();
	

		sei();

		uart_write((uint8_t*)"UART\r\n", 6);


	for(;;);

 }




