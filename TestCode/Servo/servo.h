/*
 *  servo.h
 *  
 *
 *  Created by Katherine Gunion, Anthony Estey on 24/01/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void seroInit(void);
void servoDuty(int duty);
