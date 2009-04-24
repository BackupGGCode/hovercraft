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
#include "motor/DualMotors.h"
#include "radio/radio.h"
#include "sonar/sonar.h"
#include "common.h"

#define MIN_DISTANCE 10

volatile uint8_t radio_buffer[PAYLOAD_BYTES];
volatile bool receivedInit = false;
volatile bool ackReceived  = false;
char uartBuf[255];
uint8_t uartLen = 0;

void
inline sendPing()
{
    pingPacket_t pingPacket = { PING, 1 };
    radio_send(HOV2_ADDRESS, (uint8_t *)&pingPacket);
    RADIO_SET_RECEIVE();
}

void
inline sendMovements(uint8_t rightSpeed,   uint8_t leftSpeed, 
                     direction_t rightDir, direction_t leftDir) 
{
    movePacket_t movePacket = { MOVE, rightSpeed, leftSpeed, 
                                rightDir,   leftDir};
    radio_send(HOV2_ADDRESS, (uint8_t *)&movePacket);
    RADIO_SET_RECEIVE();
}

void
inline reportToBase(uint8_t rightSpeed, uint8_t leftSpeed, uint8_t frontSonar,
                    uint8_t rightSonar, uint8_t leftSonar)
{
    infoPacket_t infoPacket = { INFO, 0, frontSonar, rightSonar, leftSonar,
                                rightSpeed, leftSpeed };
    radio_send(BASE_ADDRESS, (uint8_t *)&infoPacket);
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
    
    uint8_t sonarDistance = 0;
    
    cli();
    NO_CLK_PRESCALE();
    uart_init();
    //radio_init(HOV1_ADDRESS, RECEIVE_MODE);
    sonar_init();
    motorInit(&rightMotor);
    motorInit(&leftMotor);
    pwmInit();
    sei();
    
    setMotorDirection(&rightMotor, FORWARD);
    setMotorDirection(&leftMotor, FORWARD);
    int i;
    
    for (;;) {
        trigger_sonar(FRONT);
        sonarDistance = read_distance();
        uartLen = sprintf(uartBuf, "Distrance: %d\r\n", sonarDistance);
        uart_write((uint8_t *)uartBuf, uartLen);
        
        if (sonarDistance > MIN_DISTANCE) {
            setMotorDuty(&rightMotor, 255);
            setMotorDuty(&leftMotor, 0);
        } else {
            setMotorDuty(&rightMotor, 255);
            setMotorDuty(&leftMotor, 255);
        }
        
        for (i = 0; i < 2; ++i) _delay_ms(250);
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
        case ACK:
            ackReceived  = true;
            break;
    }
    
    /* print out the radio packet on uart */
    //uart_write((uint8_t*)radio_buffer, PAYLOAD_BYTES);
    
    /* setup the radio to receive another packet */
    radio_set_receive();
}

