/* Seng 486F Mechatronics - Mantis Cheng - Spring 2007

	Project 1
	January 2007

	Brad Barclay
	Scott Craig
	Alexander Hoole

*/

#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__
#include <avr/io.h>
#include <stdlib.h>

/*
	ADMUX register

	REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0

	Reference Selection bits

	REFS[1:0] = 01  (AVCC with external capacitor on AREF pin)

	ADC Left Adjust Result

		ADLAR = 1   (Do left justify result)

	MUX[4:0] Select pin for sample

		00000 ADC0
		00001 ADC1
		00010 ADC2
		etc. to ADC7

	We use PORTF1, PORTF2, and PORT4, as PF0 and PF3 are used on the key (Temp and VBat).

*/
#define PORTF_ENABLE_MASK		(_BV(PORTF1) | _BV(PORTF2) | _BV(PORTF4))

#define SAMP0 	(_BV(REFS0) | _BV(ADLAR) | PINF1)
#define SAMP1 	(_BV(REFS0) | _BV(ADLAR) | PINF2)
#define SAMP2 	(_BV(REFS0) | _BV(ADLAR) | PINF4)
/*
	ADCSRA register

	ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0

	ADC Enable

		ADEN = 1

	ADC Start Conversion

		Write a 1 to ADSC, ADSC will change to 0 when done

	ADIF and ADIE

		Interrupt bits (we don't use these, 00)

	ADC Prescalar bits

		ADPS[2:0] = 000 (This is max resolution. Do we need it?)
						(Could use a faster sampling rate.)
*/

/*
	Digital Input Disable Register

	DIDR0 = 11101001 (To disable ADC[7,6,5,3,0] and reduce power consumption.)
						(Leave ADC[4,2,1] enabled for our three inputs.)
*/

/*
	Procedure for sampling analog source

		1. Write to ADMUX register to select pin
		2. Write to ADCSRA register to start sample
		3. Poll ADCSRA register until sample finished
		4. Read result in ADCH and ADCL. (We use only ADCH.)

*/
#define TAKE_SAMP 	(_BV(ADEN) | _BV(ADSC))
#define ADC_NOT_DONE() 	(ADCSRA & _BV(ADSC))

enum {LEFT = 0, MIDX, RIGHT};
enum {BACK = 0, MIDY, FORWARD};

void joystick_init();
void sample_joystick(unsigned char *X, unsigned char *Y, unsigned char *T);

#endif
