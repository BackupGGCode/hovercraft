/**
 * @file Hover-StationMain.c
 * @brief The hovercraft driver.
 * 
 * @author: Katherine Gunion
 * Copyright: Copyright 2009 Katherine Gunion
 * 
 */

#include <avr/io.h>
#include <stdlib.h>
#include "common.h"
#include "Servo/servo.h"
#include "Sonar/sonar.h"

/** A macro to circumvent a bug in the radio driver. */
#define RADIO_SET_RECEIVE() do { \
                                radio_set_receive(); \
                                radio_set_receive(); \
                            } while(0)

/** The value to wait before triggering the sonar. */
#define DELAY_VAL 500

/** The timeout value that determines when to send out sonar information. */
#define TIMEOUT_VAL 5

/** The buffer that holds a packet. */
volatile uint8_t radio_buffer[PAYLOAD_BYTES];

/** The x value of the joystick. */
volatile uint8_t x = 0;

/** The y value of the joystick. */
volatile uint8_t y = 0;

/** The distance returned by the sonar driver. */
volatile uint8_t son = 0;

/** The UART buffer. */
volatile char toPrint[50];

/** The length of the string inside the UART buffer. */
volatile int len;

/** The count down for the timeout. */
volatile int timeoutCounter = 0;

/**
 * Main function.
 */
int 
main(void)
{
    cli();
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
	
    NO_CLK_PRESCALE();
	uart_init();
    motorInit(&leftMotor);
    motorInit(&rightMotor);
    pwmInit();
    
	radio_init(HOV_ADDRESS, RECEIVE_MODE);
	servoInit();
	sonar_init();
	sei();
	
    len = sprintf((char *)toPrint, "Starting ...\r\n");
    uart_write((uint8_t *)toPrint, len);
    
    setMotorDuty(&leftMotor, 255);
    setMotorDuty(&rightMotor, 255);
    
    int i=0;
    for (i=0; i<32; ++i) _delay_ms(50);
    
    for(;;) {
        
        if (y > 250 && x < 250 && x > 90) {
            setMotorDirection(&leftMotor, FORWARD);
            setMotorDirection(&rightMotor, FORWARD);  
            setMotorDuty(&leftMotor, 255);
            setMotorDuty(&rightMotor, 255);
        } else 
        
        if (y < 60 && x < 250 && x > 90) {
            setMotorDirection(&leftMotor, BACKWARD);
            setMotorDirection(&rightMotor, BACKWARD);  
            setMotorDuty(&leftMotor, 255);
            setMotorDuty(&rightMotor, 255);
        } else
        
        if (x < 80 && y > 130 && y < 230) {
            setMotorDirection(&leftMotor, BACKWARD);
            setMotorDirection(&rightMotor, FORWARD);  
            setMotorDuty(&leftMotor, 255);
            setMotorDuty(&rightMotor, 255);
        } else
        
        if (x > 250 && y > 130 && y < 230) {
            setMotorDirection(&leftMotor, FORWARD);
            setMotorDirection(&rightMotor, BACKWARD);  
            setMotorDuty(&leftMotor, 255);
            setMotorDuty(&rightMotor, 255);
        } else
        
        if (y > 200 &&  x > 220) {
            setMotorDirection(&leftMotor, FORWARD);
            setMotorDirection(&rightMotor, FORWARD);  
            setMotorDuty(&leftMotor, 255);
            setMotorDuty(&rightMotor, 245);
        } else
        
        if (y > 200 && x < 130) {
            setMotorDirection(&leftMotor, FORWARD);
            setMotorDirection(&rightMotor, FORWARD);  
            setMotorDuty(&leftMotor, 245);
            setMotorDuty(&rightMotor, 255);
        } else
        
        if (y < 100 && x < 130) {
            setMotorDirection(&leftMotor, BACKWARD);
            setMotorDirection(&rightMotor, FORWARD);  
            setMotorDuty(&leftMotor, 255);
            setMotorDuty(&rightMotor, 245);
        } else
        if (y < 200 && x > 230) {
            setMotorDirection(&leftMotor, FORWARD);
            setMotorDirection(&rightMotor, BACKWARD);  
            setMotorDuty(&leftMotor, 245);
            setMotorDuty(&rightMotor, 255);
        } else
        //if (x < 230 && x > 130 && y > 100 && y < 200)
        {
            setMotorDuty(&leftMotor, 0);
            setMotorDuty(&rightMotor, 0);
        }
        /*
		son = read_distance();
		int i = 0;

        // 2-way communication
		if (TIMEOUT_VAL <= timeoutCounter) {
			len = sprintf((char *)toPrint, "Sending: %d\r\n", son);
			uart_write((uint8_t *)toPrint, len);
			packet_t p = {son, son};
			for(i = 0; i < 2; ++i) {
				radio_send(BASE_ADDRESS, (uint8_t *)&p);
				_delay_ms(25);
			}
			timeoutCounter = 0;
            RADIO_SET_RECEIVE();
		}
        
        // Motor control with sonar override
		if (son < 12) {
			setMotorSpeed(0);
		} else {
			if( y < MIDDLE_Y){
				setMotorDirection(FORWARD);
				setMotorSpeed(255-(y*3));
			}else{
				setMotorDirection(BACKWARD);
				setMotorSpeed(y + 75);
			}
		}
        
        // Servo control
        if (x >= 130 && x < 155) {
            servoDuty(1350);
        }
		
        else if (x >= 0 && x < 65) {
            servoDuty(570);
        } else if (x >= 202 && x <= 255) {
            servoDuty(2350);
        } else if (x >=65 && x < 130) {
            servoDuty(960);
        } else if (x >= 150 && x < 202) {
            servoDuty(1850);
        }
        
		_delay_ms(DELAY_VAL);
		trigger_sonar();
		++timeoutCounter;
         */
    }
	
    return 0;   /* never reached */
}

/**
 * The ISR for the radio.
 */
ISR (INT4_vect)
{
    int i;
    
	packet_t* packet;
	PORTD ^= _BV(PORTD4);
    
    for (i = 0; i < PAYLOAD_BYTES; i++)
    {
        radio_buffer[i] = radio_get_byte();
    }
	
	packet = (packet_t*) radio_buffer;
    /* print out the radio packet on uart */
	
	int len = sprintf((char *)toPrint, "X %d, Y %d\r\n", packet->x, packet->y);
	
	x = packet->x;
	y = packet->y;
	
    uart_write((uint8_t*)toPrint, len);
    timeoutCounter = 0;

    /* setup the radio to receive another packet */
    radio_set_receive();
}

