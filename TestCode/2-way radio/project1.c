/**
 * @file project1.c
 *
 * @brief The blimp first sends a READY message to the base station. It then
 * waits in a continuous loop for a radio packet to arrive. When the packet
 * arrives, the blimp deals with it and sends a STATUS message back to the
 * base station to print to the uart and as an acknowledgement message.
 *
 * PLEASE NOTE: This file must be compiled with optimisation at least -O2
 * else delay code library & radio communication will not work properly.
 *
 * @n @n Seng 466 Software for Mechatronic and Embedded Systems - Spring 2008
 *
 * @author Leanne Ross
 * @author Mantis Cheng
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "base/uart.h"
#include "common.h"
#include "radio.h"


/*------------Global Variables and Constants ----------------------------------*/

/** The buffer to hold the packet contents. */
volatile uint8_t radio_buf[PAYLOAD_BYTES];
/* Length of buffer holding radio packet. */
volatile uint8_t packet_available;


/*---------------- Function Definitions ---------------------------------------*/
void init_global()
{
   /** Disable default prescaler to make processor speed 8 MHz. */
   clock8MHz();

   uart_init(1);
   sei(); /** enable global interrupts **/
   /* grab register content
      send interrupt to clear register
      send next content */
}

int main()
{

   /*--------------------- Global Initialization ------------------------------------*/
   init_global();
   radio_init((uint8_t) RADIO_CHANNEL, BLIMP_ADDRESS, RX_MODE);
   packet_available = 0;

   /* Turn off LED */
   PORTD &= ~LED_D5_RED;

   /*--------------------- Initial State of Blimp -----------------------------------*/
   uart_println("Starting blimp ...\r\n");
   int i,j;
   packet p, *q;		// p is sending, *q is receiving

   p.type = READY;
   strncpy(p.payload.msg, "Blimp Connected", 16);//sizeof(p.payload.msg));
 
   for (i=0; i<3; i++)
   {
      radio_send(BASE_ADDRESS, (uint8_t*) &p);
      for (j=0; j<4; j++)
	  {
         _delay_ms(25);
      }
   }

   /*--------------------- Response / Listen State of Blimp -------------------------*/
   for (;;)
   {
      radio_set_receive();

      /*---------------- wait for radio message from base station -------------------*/
      while (!packet_available);

	  uart_println("in blimp: got packet\r\n");
      q = ((packet*) radio_buf);		// q points to radio_buf
      switch (q->type)
      {
         case CONTROL:
            p.payload.status.lastHeight=q->payload.control.X;
            p.payload.status.currHeight=q->payload.control.Y;
            p.payload.status.heightDiff=q->payload.control.Z;
            p.type=STATUS;
            radio_send(BASE_ADDRESS, (uint8_t*) &p);
            break;
         case DEBUG:
         case STATUS:
         case READY:
         default:
            p.type = READY;
            strncpy(p.payload.msg, "Blimp Connected", 16);
            radio_send(BASE_ADDRESS, (uint8_t*) &p);
            break;
      }
         packet_available = 0;
   }
} // end of main

