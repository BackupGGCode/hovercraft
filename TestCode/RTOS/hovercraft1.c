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

#define MIN_DISTANCE 10
#define LISTEN 2
#define FIRE 1
#define MOTOR 3
#define STOP 5
#define RADIO 4

volatile uint8_t radio_buffer[PAYLOAD_BYTES];
volatile bool receivedInit = false;

char uartBuf[255];
uint8_t uartLen = 0;

EVENT* stop;
const unsigned char PPP[] = {FIRE, 4, LISTEN, 1, FIRE, 4, LISTEN, 1, FIRE, 4, LISTEN, 1, MOTOR 5};
const unsigned int PT = 6;

volatile uint8_t s_right = 0;
volatile uint8_t s_left = 0;
volatile uint8_t s_forward = 0;
volatile uint8_t send = 0;

volatile uint8_t l_dir;
volatile uint8_t r_dir;
volatile uint8_t l_duty;
volatile uint8_t r_duty;





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


void 
fire_sonar(void)
{

	for(;;){
	 	
		trigger_sonar(FRONT);
		Task_Next();
		trigger_sonar(RIGHT);
		Task_Next();
		trigger_sonar(LEFT);
		Task_Next();
	}
}


void
listen_sonar(void)
{
	for(;;)
	{
		
	    s_forward= read_distance();


	    Task_Next();
	    s_right = read_distance();
	    Task_Next();
	    s_left = read_distance();
	    Task_Next();
	}
}


void
motor_task(void)
{
	for(;;){
    	if(s_front < MIN_DISTANCE &&  s_left >MAX_DISTANCE)
    	{
			r_dir= FORWARD;
			r_duty = 255;
			l_dir = BACKWARD;
			l_duty = 255;
		
    		   
   		} else if (s_front < MIN_DISTANCE && s_right>MAX_DISTANCE) {
	

			r_dir = BACKWARD;
			r_duty = 255;
			l_dir= FORWARD;
			l_duty = 255;					
			
			
    	}else if(s_left < MIN_DISTANCE && s_right > MAX_DISTNACE){
			l_dir = FORWARD;
			l_duty = 255;
			r_duty = 0;
			
   	 	}else if(s_right<MIN_DISTANCE && s_left > MAX_DISTANCE){
			r_dir = BACKWARD;
			r_duty = 255;
			l_duty = 0;			
		
	
    	}else{
			Signal_Event(stop);
    	}
			
			setMotorDirection(&rightMotor,r_dir;
			setMotorDuty(&rightMotor, r_duty);
			setMotorDirection(&leftMotor,l_dir);
			setMotorDuty(&leftMotor, l_duty);
	}

}

void
sendRadio(void){

    for(;;){
	sendMovements(r_duty,l_duty,r_dir,r_duty);
	reportToBase(r_speed,l_speed, s_forward, s_right, s_left);

	send ++;
	if(send == 3)
	    Event_Signal(stop);
	
	Task_Next();

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
    
    //uint8_t sonarDistance = 0;
    
    cli();
    NO_CLK_PRESCALE();
    //uart_init();
    //radio_init(HOV1_ADDRESS, RECEIVE_MODE);
    sonar_init();
    motorInit(&rightMotor);
    motorInit(&leftMotor);
    pwmInit();
    sei();
    
	stop = Event_Init();

    for (;;) {
        // Wait unit the initiate message is sent from the base.
         if (!receivedInit) continue;
      
    }
    sendPing();
    while(!receivedInit);

    Task_Create((void*)(&motor_task),0, PERIODIC, MOTOR);
    Task_Create((void*)(&fire_sonar),0, PERIODIC, FIRE);
    Task_Create((void*)(&listen_sonar),0,PERIODIC, LISTEN);
    Task_Create((void*)(&sendRadio),0,PERIODIC, RADIO);
	Task_Create((void*)(&stopSystem), ),SYSTEM, STOP); 
   
    setMotorDirection(&rightMotor, FORWARD);
    setMotorDirection(&leftMotor, FORWARD);
    
    
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
	case PONG:
	    receivedPong = true;
	    break;
	case ACK:
		send = 0;
		break;

    }
    
    /* print out the radio packet on uart */
    //uart_write((uint8_t*)radio_buffer, PAYLOAD_BYTES);
    
    /* setup the radio to receive another packet */
    radio_set_receive();
}

