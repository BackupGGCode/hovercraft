/**
 * @file   common.h
 * @author Scott Craig
 * @author Justin Tanner
 *
 * @brief Common macros.
 *
 * CSC 460/560 Real Time Operating Systems - Mantis Cheng
 *
 */

#ifndef __COMMON_H__
#define __COMMON_H__

/** disable default prescaler to make processor speed 8 MHz. */
#define CLOCK_8MHZ()            CLKPR = (1<<CLKPCE); CLKPR = 0x00;

/** CPU frequency needed by util/delay.h */
// #define F_CPU                   8000000UL  // 8 MHz

#define DISABLE_INTERRUPTS()    asm volatile ("cli"::)
#define ENABLE_INTERRUPTS()     asm volatile ("sei"::)

#endif
