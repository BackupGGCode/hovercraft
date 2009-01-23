#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <math.h>

#include "../common.h"
#include "Motor.h"



int main(int argc, char**argv)
{
//-----------------Initialization -----------------------//
    DISABLE_INTERRUPTS();
    

	CLOCK_8MHZ();
	motor_init();
	ENABLE_INTERRUPTS();
	setMotorON();



	for(;;){

		setMotorSpeed(150);
		





	


	}

}
