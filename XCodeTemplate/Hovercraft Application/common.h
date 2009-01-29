/**
 * @file   common.h
 * @brief Common macros.
 *
 * Project: ÇPROJECTNAMEÈ
 *
 */


#ifndef __COMMON_H__
#define __COMMON_H__

#define NO_CLK_PRESCALE() CLKPR=(1<<CLKPCE); \
                          CLKPR=0x00

#endif
