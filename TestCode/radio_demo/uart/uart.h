/** CSC 460/560 Real Time Operating Systems - Mantis Cheng
 *	Fall 2007
 *	
 *	@author Scott Craig
 *
 *	A driver for sending debugging messages from one radio
 *	to another. Delivery is not guaranteed.
 *
 *  NB!: Must be compiled with optimisation at least -O2
 */

#ifndef __UART_INT_H__
#define __UART_INT_H__

#include <avr/interrupt.h>

#define Disable_Interrupt() 	asm volatile ("cli"::)
#define Enable_Interrupt() 	asm volatile ("sei"::)

#define BAUD			38400

#define UART_TX_BUF_MASK    255

#if (UART_TX_BUF_MASK & (UART_TX_BUF_MASK + 1))
#error UART_TX_BUF_MASK must be 1 less than a power of 2
#endif

#define UCSR1A_CFG		(_BV(U2X1))

#define UCSR1B_CFG		( _BV(TXEN1))

#define UCSR1C_CFG		(_BV(UCSZ11) | _BV(UCSZ10))

/// UBRRn = f_OSC * (1 + value(U2Xn)) / (16 * BAUD) - 1  in floating point
#define UBRR1_CFG	\
(((	\
	F_CPU * (((UCSR1A_CFG & _BV(U2X1)) >> U2X1) + 1)	\
	/ 8 / BAUD	\
) - 1) / 2)

#define UBRR1H_CFG		(UBRR1_CFG >> 8)		
#define UBRR1L_CFG		(UBRR1_CFG & 0xFF)

#define TXIntEnable()	UCSR1B |= _BV(UDRIE1)	
#define TXIntDisable()	UCSR1B &= ~(_BV(UDRIE1))
#define Is_TXIntDisabled()	(!(UCSR1B & _BV(UDRIE1)))	

/** Initialize the uart driver */
void uart_init(void);
/** Write data to the uart. */
int uart_write(uint8_t* const str, int len);

#endif
