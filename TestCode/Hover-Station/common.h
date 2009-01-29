/**
 * @file   common.h
 * @brief Common macros.
 *
 * Project: Hover-Station
 *
 */


#ifndef __COMMON_H__
#define __COMMON_H__

#include "radio/radio.h"
#include "../uart/uart.h"
#include "../Motor/Motor.h"

#define BASE_ADDRESS 0xDEAD
#define HOV_ADDRESS  0xFACE

#define NO_CLK_PRESCALE() CLKPR=(1<<CLKPCE); \
                          CLKPR=0x00

#endif
