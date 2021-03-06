/**
 * @file   common.h
 *
 * @brief Some common macros.
 *
 * CSC 460/560 Real Time Operating Systems - Mantis Cheng
 *
 * @author Scott Craig
 * @author Justin Tanner
 */


#ifndef __COMMON_H__
#define __COMMON_H__

#include "Hover-Station/radio/radio.h"

#define CLOCK_8MHZ()            CLKPR = (1<<CLKPCE); CLKPR = 0x00;

//#define F_CPU                   8000000UL  // 8 MHz


#define Disable_Interrupt()     asm volatile ("cli"::)
#define Enable_Interrupt()     asm volatile ("sei"::)


#define BASE_ADDRESS 0xDEAD
#define HOV_ADDRESS  0xFACE

#endif
