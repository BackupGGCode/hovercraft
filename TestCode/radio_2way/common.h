/**
 * @file   common.h
 * @brief Common macros.
 *
 * Project: TX_RX
 *
 */


#ifndef __COMMON_H__
#define __COMMON_H__

#define NO_CLK_PRESCALE() CLKPR=(1<<CLKPCE); \
                          CLKPR=0x00

#endif
