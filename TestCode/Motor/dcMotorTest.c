#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../common.h"
#include "Motor.h"



int main(int argc, char**argv)
{
    //-----------------Initialization -----------------------//
    cli();
	CLOCK_8MHZ();
	motor_init();
	sei();

    //----------------- Control -----------------------//
	setMotorON();
	for(;;){
		setMotorSpeed(235);
	}

}
