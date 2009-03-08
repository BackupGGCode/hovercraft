/**
 * @file   common.h
 * Common macros and addresses.
 *
 * Project: Hover-Station
 *
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include "radio/radio.h"
#include "../uart/uart.h"
#include "../DualMotors/DualMotors.h"
#include <util/delay.h>
#include <avr/interrupt.h>

/** The address for the base station. */
#define BASE_ADDRESS 0xDEAD

/** The address for the hovercraft. */
#define HOV_ADDRESS  0xFACE

/** Turn off clock prescaler. */
#define NO_CLK_PRESCALE() CLKPR=(1<<CLKPCE); \
                          CLKPR=0x00

#endif /* __COMMON_H__ */
