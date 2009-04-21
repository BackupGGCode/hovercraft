/*
 *  hovercraft2.c
 *  RTOS
 *
 *  Created by Marco Yuen on 20/04/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include <stdbool.h>
#include "uart/uart.h"
#include "radio/radio.h"
#include "motor/DualMotors.h"
#include "sonar/sonar.h"
#include "common.h"

volatile uint8_t radio_buffer[PAYLOAD_BYTES];

volatile bool receivedInit = false;

void inline 
sendPing()
{
    pingPacket_t pingPacket = { PING, 1 };
    radio_send(HOV2_ADDRESS, (uint8_t *)&pingPacket);
    RADIO_SET_RECEIVE();
}

int
main(int argc, char *argv[])
{
    motor_t rightMotor = {
        &PORTB, &DDRB, PORTB7, // PWM
        &PORTC, &DDRC, PORTC1, // Direction
        &OCR0A                 // Top value
    };
    
    motor_t leftMotor = {
        &PORTD, &DDRD, PORTD0, // PWM
        &PORTB, &DDRB, PORTB0, // Direction
        &OCR0B                 // Top value
    };
    
    cli();
    NO_CLK_PRESCALE();
    uart_init();
    radio_init(HOV1_ADDRESS, RECEIVE_MODE);
    sonar_init();
    motorInit(&rightMotor);
    motorInit(&leftMotor);
    pwmInit();
    sei();
    
    for (;;) {
        // Wait unit the initiate message is sent from the base.
        if (!receivedInit) continue;
    }
    
    return 0;
}

ISR (INT4_vect)
{
    int i;
    
    PORTD ^= _BV(PORTD5);
    
    for (i = 0; i < PAYLOAD_BYTES; i++)
    {
        radio_buffer[i] = radio_get_byte();
    }
    
    // Figure out what type of packet this is.
    genericPacket_t *incomingPacket = (genericPacket_t *)radio_buffer;
    switch(incomingPacket->type) {
        case INIT:
            receivedInit = true;
            break;
    }
    
    /* print out the radio packet on uart */
    //uart_write((uint8_t*)radio_buffer, PAYLOAD_BYTES);
    
    /* setup the radio to receive another packet */
    radio_set_receive();
}

