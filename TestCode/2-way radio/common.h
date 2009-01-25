/**
 * @file   common.h
 *
 * @brief Common macros and structures.
 *
 * CSC 460/560 Real Time Operating Systems - Mantis Cheng
 *
 * @author Scott Craig
 * @author Justin Tanner
 *
 * Changes made by Leanne Ross in collaboration with
 * Mantis Cheng - January 2008
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include "radio.h"

/** Disable default prescaler to make processor speed 8 MHz. */
#define clock8MHz()   CLKPR = (1<<CLKPCE); CLKPR = 0x00;

// #define F_CPU   8000000UL  // 8 MHz

#define Disable_Interrupt()  asm volatile ("cli"::)
#define Enable_Interrupt()   asm volatile ("sei"::)

#define LED_D5_RED 0x80

/* define type for radio packets */
typedef enum
{
   READY=1,
   CONTROL=2,
   STATUS=3,
   DEBUG=4
} packet_type;

/* define radio packet */
typedef struct _packet
{
   uint8_t type;	// type of radio packet
   union
   {
      struct		// joystick information
      {
         uint8_t X;
         uint8_t Y;
         uint8_t Z;
      } control;

      struct		// status of blimp
      {
         uint8_t currHeight;
         uint8_t lastHeight;
         uint8_t heightDiff;
      } status;

      char msg[PAYLOAD_BYTES-sizeof(uint8_t)];		// use for debugging only

   } payload;

} packet;

#endif
