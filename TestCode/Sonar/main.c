/*
 *  main.c
 *  Sonar
 *
 *  Created by Katherine Gunion on 25/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include "../common.h"
#include "sonar.h"
#include <avr/io.h>
 int	
 main(void){

	DISABLE_INTERRUPTS();
		DDRD  = 0xff;
		PORTD = 0xff;  // Turn on both LEDs
		sonarInit();

	ENABLE_INTERRUPTS();


	for(;;);

 }




