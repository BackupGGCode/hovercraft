/**	@file joystick.c
 *	@brief Joystick Driver
 *
 *	NB!: Seng 486F Mechatronics - Mantis Cheng - Spring 2007
 *
 *	@n Project 1
 *	@n January 2007
 *
 *	@author Brad Barclay
 *	@author Scott Craig
 *	@author Alexander Hoole
 *
 * 	Modifications by Aaron Kaspar, Colburn Holbrook and Josh Fairhead, 
 * 	spring 2008
 */

#include "joystick.h"

#define NUM_SAMPLES		11
// File scope variables 
static unsigned char next_sample;
static int sum[3];

unsigned char normX[3] = {0, 128, 255};
unsigned char normY[3] = {0, 128, 255};


// Forward declarations
static void single_sample(unsigned char SAMPN, int input_num);

/**
 * @brief Initialized interface for the joystick
*/
void joystick_init()
{
	int i = 0;

	/* Set up input pins */
	DIDR0 = ~PORTF_ENABLE_MASK; // Disable unused ADC input pins
	DDRF &= ~PORTF_ENABLE_MASK; // Set low for input
	PORTF &= ~PORTF_ENABLE_MASK;  // Write 0 to disable pull-up resistors

	// Initialise sample[][] and sum[]
	for(i = 0; i < NUM_SAMPLES; ++i)
	{
		single_sample(SAMP0, 0);
		single_sample(SAMP1, 1);
		single_sample(SAMP2, 2);
		++next_sample;
		if (next_sample >= NUM_SAMPLES) next_sample = 0;
	}

}


/**
 * @brief take a single sample of the specified joystick axis
 *
 * Samples each axis three times consecutively in order to overcome
 * a memory issue in the A/D converter of the ATMEL chip
 */
static
void single_sample(unsigned char SAMPN, int input_num)
{
	ADMUX = SAMPN;

	//need to sample multiple times, because A/D seems to have some
	//memory of the previous value that was read

	ADCSRA = TAKE_SAMP;
	while (ADC_NOT_DONE()){}
	ADCSRA = TAKE_SAMP;
	while (ADC_NOT_DONE()){}
	ADCSRA = TAKE_SAMP;
	while (ADC_NOT_DONE()){}


	//removed averagving code since our joystick is stable

	sum[input_num] = ADCH;
}


/**
 *	@brief Takes a sample of the joystick values and stores them.
 */
void sample_joystick(unsigned char *X, unsigned char *Y, unsigned char *T)
{

	single_sample(SAMP0, 0);
	*X = (unsigned char) (sum[0] );

	single_sample(SAMP1, 1);
	*Y = (unsigned char) (sum[1] );

	single_sample(SAMP2, 2);
	*T = (unsigned char) (sum[2] );

	++next_sample;
	if (next_sample >= NUM_SAMPLES) next_sample = 0;
}
