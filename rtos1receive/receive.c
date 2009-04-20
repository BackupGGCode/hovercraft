/**
 * @file   receive.c
 *
 * @brief  Receive input from the temperature sensor over radios,
 *         and control the duty applied to the motor
 *
 * CSC 460/560 Real Time Operating Systems
 *
 * @author Cecelia Redding
 * @author Matthew Campbell
 */

#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "os.h"
#include "common.h"
#include "uart.h"
#include "radio.h"


// Wireless Radio Addresses
#define RECEIVE_ADDRESS	0x0042
#define SEND_ADDRESS	0x0052

// Task Name Define
#define RECEIVE 1

// PPP Defines
const unsigned char PPP[] = {RECEIVE, 50};
const unsigned int PT = 1;

// Motor Direction Definitions
typedef enum {
	BACKWARD = 1,
	FORWARD = 0
} DIRECTION;

// Motor Min/Start Duty (indexed by motor DIRECTION)
volatile int minDuty[2];
volatile int startDuty[2];

// Store the motor's state: direction and duty
volatile int motorStatus[2];

// Store the desired temperature
volatile int desiredTemp;

// Store the radio input
volatile uint8_t radio_buffer[PAYLOAD_BYTES];

// Store the actual temperature received from the radio
volatile int actualTemp;

/**
 * void pwmInit()
 *   Initialize the PWM used to run the motor.
 * Set direction and enable ports (PORTB3, PORTB4)
 * Set status registers (TCCR2A, TCCR2B)
 */
void pwmInit()
{
	// Set PORTB3 (direction) and PORTB4 (enable) pins for input
	DDRB |= (1<<PORTB3) | (1<<PORTB4);

	// Set WGM20/WGM21 (waveform generation mode) to 'fast pwm'
	// Set COM2A1 (compare output mode) to 'count up and clear on match'
	TCCR2A = (1<<WGM20) | (1<<WGM21) | (1<<COM2A1);

	// Set CS20 (clock frequency bit) to 'no prescaling'
	TCCR2B = (1<<CS20);
}

/**
 * void setMinDuty( unsigned int duty, DIRECTION d )
 *   Set the minimum duty (in percentage) the motor can sustain.
 * Performs some boundary checks: cannot be less than 0% or more than 100%.
 */
void setMinDuty( unsigned int duty, DIRECTION d )
{
	// Boundary checks (0 <= duty <= 100)
	duty = (duty > 100) ? 100 : duty;

	// Store the duty
	minDuty[d] = duty;
}

/**
 * void setStartDuty( unsigned int duty, DIRECTION d )
 *   Set the startup duty (in percentage) the motor needs to run.
 * Performs some boundary checks: cannot be less than 0% or more than 100%.
 */
void setStartDuty( unsigned int duty, DIRECTION d )
{
	// Renormilize the input duty to be within the range min - 100%
	int range, min;
	min = minDuty[d];

	if( duty != 0) {
		range = 100 - min;
		duty = min + ((double)duty/100)*range;
	}

	// Boundary checks (0 <= duty <= 100)
	duty = (duty > 100) ? 100 : duty;

	// Save the duty
	startDuty[d] = duty;
}

/**
 * void setCurrentDuty( unsigned int duty )
 *   Set the motor's duty (in percentage).
 * The given percentage is adjusted to be above the min duty.
 * The motor is started (if stopped) before the given duty is applied.
 */
void setCurrentDuty( unsigned int duty )
{
	// Get the min and start duty for the current direction
	int min   = minDuty[motorStatus[0]];
	int start = startDuty[motorStatus[0]];

	// Save incoming duty
	int inDuty = duty;

	// Adjust input to available range, if not stopping motor
	int range;
	if( duty != 0) {
		range = 100 - min;
		duty = min + ((double)duty/100)*range;
	}

	// Startup motor if currently stopped
	if( (motorStatus[1] == 0) && (inDuty != 0) ) {
		OCR2A = start * 2.55;
		Task_Next();
	}

	// Calculate new duty value the motor can understand
	int regDuty = ((float)duty / 100) * 2.55 * (100 - min) + (min * 2.55);

	// Boundary checks
	regDuty = (regDuty > 255) ? 255 : regDuty;
	regDuty = (regDuty <= min * 2.55) ? 0 : regDuty;

	// Set the new duty
	OCR2A = regDuty;
	motorStatus[1] = inDuty;
}

/**
 * void setDirection( DIRECTION d )
 *   Set the motor direction (FORWARD or BACKWARD).
 * Set the duty to 0 before switching directions.
 */
void setDirection( DIRECTION d )
{
	// Return if not changing directions
	if( motorStatus[0] == d ) return;

	// Clear the duty and allow motor to stop before changing directions
	setCurrentDuty(0);
	//Task_Next();

	if( motorStatus[0] == BACKWARD ) {
		PORTB &= !(1<<PORTB3);
	} else {
		PORTB |= (1<<PORTB3);
	}

	// Save the state
	motorStatus[0] = d;
}

/**
 * void updateMotorFromTemp( void )
 *   Determine the direction and duty for the motor, and print status message.
 */
void updateMotorFromTemp( void )
{
	int  tempDifference, nextDuty;
	DIRECTION nextDirection;

	// Calculate the actual and desired temperature difference
	tempDifference = actualTemp - desiredTemp;

	// If it's too cold, the motor should run BACKWARD
	if( tempDifference <= 0 ) {
		nextDirection = BACKWARD;
		tempDifference = -tempDifference;

	// If it's too hot, the motor should run FORWARD
	} else {
		nextDirection = FORWARD;
	}

	// Turn the temperature difference into a duty percentage
	nextDuty = 25 * tempDifference;
	if( nextDuty > 100 ) {
		nextDuty = 100;
	}

	// If the motor state has changed, print status and save new state
	if( (motorStatus[0] != nextDirection) || (motorStatus[1] != nextDuty) )
	{
		char output[80];
		memset(output, 0, 80);
		sprintf(output, "DesiredTemp=%d, ActualTemp=%d, Direction=%d, Duty=%d\r\n", desiredTemp, actualTemp, nextDirection, nextDuty);
		uart_putstringln(output);

		setDirection(nextDirection);
		setCurrentDuty(nextDuty);
	}
}

/**
 * void receive( void )
 *   Poll for UART input and then update the motor direction and duty.
 */
void receive( void )
{
	// Start polling for UART input to change desired temperature
	// Note: We check for 'P' and 'j' because our UART system was being funny.
	for(;;)
	{
		// Check for input
		char input = uart_getchar();
		if( input != 255 )
		{
			// Decrease desired temperature
			if( (input == 'P') || (input == '-') ) {
				desiredTemp--;
				uart_putstringln("Descreased Desired Temperature\r\n");

			// Increase desired temperature
			} else if( (input == 'j') || (input == '+') ) {
				desiredTemp++;
				uart_putstringln("Increased Desired Temperature\r\n");
			}
		}

		updateMotorFromTemp();
		Task_Next();
	}
}

/**
 * int main( void )
 *   Setup motor state, desired temperature and start RECEIVE task.
 */
int main( void )
{
	// Disable interrupts for initializations
	Disable_Interrupt();

	// Set the clock frequency
	clock8MHz();

	// Initialize the UART
	uart_init(1);

	// Initialize the motor PWM
	pwmInit();

	// Initialize the radio to receive
	radio_init(RECEIVE_ADDRESS, RECEIVE_MODE);

	// Set min and start duty for the motor
	// FORWARD and BACKWARD have different min!
	setMinDuty(30, FORWARD);
	setMinDuty(30, BACKWARD);
	setStartDuty(100, FORWARD);
	setStartDuty(100, BACKWARD);

	setDirection(FORWARD);
	setCurrentDuty(0);

	// Set the default temperature
	desiredTemp = 27;
	actualTemp = desiredTemp;

	int success = Task_Create((void*)(&singleTask), 0, PERIODIC, RECEIVE);
	if( success == 0 )
	{
		// Turn LED on for error
		DDRD = 0xff;
		PORTD = 0xff;
		for(;;);
	}

	// Re-enable interrupts to continue
	Enable_Interrupt();
}

/**
 * ISR( INT4_vect )
 *   Radio interrupt.
 * Reads the data from the radio, which contains the temperature.
 * Sets global to be read by a task.
 */
ISR( INT4_vect )
{
	Disable_Interrupt();

	int temp;
    int i;
    for( i=0; i<PAYLOAD_BYTES; i++) {
        radio_buffer[i] = radio_get_byte();
    }
	temp = atoi((char*)radio_buffer);

	actualTemp = temp;

    // Setup the radio to receive another packet
    radio_set_receive();

	Enable_Interrupt();

}
