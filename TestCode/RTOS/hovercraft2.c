/*
 *  hovercraft2.c
 *  RTOS
 *
 *  Created by Marco Yuen on 20/04/09.
 *  Copyright 2009 University of Victoria. All rights reserved.
 *
 */

#include "uart/uart.h"
#include "motor/DualMotors.h"
#include "radio/radio.h"
#include "common.h"
#include <stdbool.h>

volatile uint8_t radio_buffer[PAYLOAD_BYTES];
volatile bool pingReceived = false;
volatile bool needSendAck  = false;
volatile bool moveRecieved = false;
volatile uint8_t rightMotorSpeed = 0;
volatile uint8_t leftMotorSpeed  = 0;
volatile uint8_t rightDirection = FORWARD;
volatile uint8_t leftDirection = FORWARD;

EVENT* ack;
EVENT* pong;

#define PONG 1
#define ACK 0
#define MOTORS 3
#define STOP 2


void inline sendPong() {
    pongPacket_t pongResponse = { PONG, 1 };
    radio_send(HOV1_ADDRESS, (uint8_t *) &pongResponse);
    radio_set_receive();
    radio_set_receive();
}

void inline sendAck() {
    ackPacket_t ackResponse = { ACK, 1 };
    radio_send(HOV1_ADDRESS, (uint8_t *) &ackResponse);
    radio_set_receive();
    radio_set_receive();
}




void
ack_task(void){
	for(;;){
		Event_Wait(ack);
		sendACK;
	}
}

void
pong_task(void){


	Event_Wait(pong);
	sendPong();

}

void
up_motors(void){

	for(;;){
		Event_Wait(ack);
	    setMotorDuty(&rightMotor, rightMotorSpeed);	
        setMotorDuty(&leftMotor, leftMotorSpeed);
		setMotorDirection(&rightMotor, rightDirection);
		setMotorDirection(&leftMotor, leftDirection);
	}		
}	

void
stopSystem(void){

	Event_Wait(stop);
	sendMovements(0,0,FORWARD,FORWARD);
	exit(0);



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
    radio_init(HOV2_ADDRESS, RECEIVE_MODE);
    motorInit(&rightMotor);
    motorInit(&leftMotor);
    pwmInit();
    sei();
	
	ack = Event_Init();
	pong = Event_Init();
	stop = Event_Init();


	Task_Create((void*)(&ack_task),0, RR, ACK);
    Task_Create((void*)(&up_motors),0, RR, MOTORS);
	Task_Creat((void*)(&pong_task),0,RR,PONG);
	Task_Create((void*)(&stopSystem), 0,SYSTEM, STOP); 


    
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
    movePacket_t *theMovePacket;
    switch(incomingPacket->type) {
        case PING:
            pingReceived = true;
			Signal_Event(pong);
            break;
        case MOVE:
            theMovePacket = (movePacket_t *) radio_buffer;
            rightMotorSpeed = theMovePacket->rightMotor;
            leftMotorSpeed  = theMovePacket->leftMotor;
			rightDirection= theMovePacket-> rightDirection;
			leftDirection = theMovePacket -> leftDirection;
			Event_Broadcast(ack);
            needSendAck = true;
            break;
        default:
            // ignore
            break;
    }
    
    /* print out the radio packet on uart */
    //uart_write((uint8_t*)radio_buffer, PAYLOAD_BYTES);
    
    /* setup the radio to receive another packet */
    radio_set_receive();
}

