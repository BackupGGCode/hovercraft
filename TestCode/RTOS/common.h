/**
 * @file   common.h
 * @brief Common macros.
 *
 * Project: RTOS
 *
 */


#ifndef __COMMON_H__
#define __COMMON_H__

#define NO_CLK_PRESCALE() CLKPR=(1<<CLKPCE); \
                          CLKPR=0x00

#define Disable_Interrupt() asm volatile ("cli"::)
#define Enable_Interrupt()  asm volatile ("sei"::)

#endif
