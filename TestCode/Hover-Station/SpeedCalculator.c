/*
 *  SpeedCalculator.c
 *  Hover-Station
 *
 *  Created by Marco Yuen on 15/03/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "SpeedCalculator.h"
#include "../uart/uart.h"
#include "../Sonar/sonar.h"

#define OVERFLOW_LIMIT 30
#define TIME_BETWEEN_SONAR 2

volatile int overflowCounter  = 0;
volatile uint8_t timeInterval = 0;
volatile uint8_t distances[2] = {0, 0};
volatile bool isSecondReading = false;

volatile char uartBuf[255];
volatile int  uartLen = 0;

void 
speedCalcInit()
{
    // Set TIMER 2 to CTC mode.
    TCCR2A &= ~(1<<WGM20);
    TCCR2A |=(1<<WGM21);
    TCCR2B &= ~(1<<WGM22);
    
    // Set clock prescaler to 1024
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    
    // Set OC2A on compare match
    TCCR2A |= (1 << COM2A1) | (1 << COM2A0);
    
    // Enable interrupt on compare match
    TIMSK2 |= (1 << OCIE2A);
    
    // Enable overflow interrupt
    TIMSK2 |= (1 << TOIE2);
    
    // Set OCR2A to the highest possible value.
    OCR2A = 0xFF;
}

/**
 * Compare match interrupt service routine.
 */
ISR(TIMER2_COMPA_vect)
{
    
}

/**
 * Timer 2 overflow interrupt service routine.
 */
ISR(TIMER2_OVF_vect)
{
    if (++overflowCounter >= OVERFLOW_LIMIT) {
        ++timeInterval;
    }
    
    if (timeInterval >= TIME_BETWEEN_SONAR) {
        trigger_sonar();
        timeInterval = 0;
        overflowCounter = 0;
        
        if (!isSecondReading) {
            distances[0] = read_distance();
            isSecondReading = true;
        } else {
            distances[1] = read_distance();
            
            uartLen = sprintf((char *)uartBuf, "%d\r\n", distances[0]);
            uart_write((uint8_t *)uartBuf, uartLen);
            uartLen = sprintf((char *)uartBuf, "%d\r\n", distances[1]);
            uart_write((uint8_t *)uartBuf, uartLen);
            
            // calcluate velocity
            int velocity = (distances[1] - distances[0])/TIME_BETWEEN_SONAR;
            uartLen = sprintf((char *)uartBuf, "vel: %d\r\n", velocity);
            uart_write((uint8_t *)uartBuf, uartLen);
            
            distances[0] = distances[1];
        }
    }
}
