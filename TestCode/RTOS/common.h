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

#define BASE_ADDRESS 0xDEAD
#define HOV1_ADDRESS 0xFACE
#define HOV2_ADDRESS 0xBEEF

#define RADIO_SET_RECEIVE() radio_set_receive(); \
                            radio_set_receive()

#endif
