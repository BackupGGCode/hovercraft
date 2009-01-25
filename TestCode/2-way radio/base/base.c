/**
 * @file base.c
 *
 * @brief The base station acts as the controller. It sends a message to the
 * blimp then waits for a status acknowledgement receipt and prints the status
 * to a terminal screen through UART.
 *
 * PLEASE NOTE: This file must be compiled with optimisation at least -O2
 * else delay code library & radio communication will not work properly.
 *
 * @n @n Seng 466 Software for Mechatronic and Embedded Systems - Spring 2008
 *
 * @author Leanne Ross
 * @author Mantis Cheng
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#include "../common.h"
#include "../radio.h"
#include "uart.h"

#define LED_D5_RED        	0x80			/* Port D, Pin 7 */

/*--------------------- Global Variables and Constants ------------------------------*/
/** The buffer to hold the packet contents. */
volatile uint8_t radio_buf[PAYLOAD_BYTES];
/** The flag for a radio packet has been received (1) and is ready to be processed. */
volatile uint8_t packet_available;
/** The period of time to delay in main loop. */
#define POLL_INTERVAL 25
/** The number of times to complete the loop before timeout of receive mode. */
#define TIME_OUT 1000/POLL_INTERVAL

/* define type for current state */
typedef enum
{
   INIT=1,
   CONTROLLER=2,
   LISTEN=3
} current_state;

/** The current state of the base station. */
uint8_t cState;


/*--------------------- Function Definitions ----------------------------------------*/
/**
 * @brief Main function
 *  Wait for radio packets and write the contents to UART.
 *  Send radio packets consisting of joystick coordinates.
 */
int main(int argc, char**argv)
{

   /*--------------------- Global Initialization ------------------------------------*/
   /* Disable default prescaler to make processor speed 8 MHz. */
   clock8MHz();

   /* Initialize UART */
   uart_init(1);

   /* Initialize Radio */
   radio_init((uint8_t) RADIO_CHANNEL, BASE_ADDRESS, RX_MODE);
   packet_available = 0;

   /* Initialize an LED on PORTD7 */
   PORTD ^= LED_D5_RED;
   DDRD |= LED_D5_RED;

   Enable_Interrupt();
   cState = INIT;

   /*--------------------- Main For Loop of Base Station ----------------------------*/

   uart_println("Base Station Connected\r\n");
   packet p, *q;		// p is sending, *q is receiving
   int count=0;

   /* Initialize "data" being sent to blimp */
   p.payload.control.X=25;
   p.payload.control.Y=50;
   p.payload.control.Z=75;

   /* Packet structure must = PAYLOAD_BYTES otherwise radio will not work propertly */
   if (sizeof(packet) != PAYLOAD_BYTES)
   {
      uart_println( "Incorrect packet size (%d)!\n", sizeof(packet) );
      return 0;
   }

   for (;;)
   {
      switch(cState)
      {
         case(INIT):
            uart_println("Init cState: %u\r\n",cState);
            /* Wait for ready message from blimp */
            while(!packet_available);
            q = ((packet*) radio_buf);		// q points to radio_buf which is in packet format
            switch(q->type)
            {
               case READY:
                  uart_println("\t%s\r\n",q->payload.msg);
			      cState=LISTEN;
				  radio_set_receive();
			      count=0;
                  break;
               case STATUS:
               case CONTROL:
               case DEBUG:
               default:
                  uart_println("\tBlimp not ready!\r\n");
                  return(0);
            }
            packet_available = 0;
            break;
         case(LISTEN):
            if (packet_available)
            {
               q = ((packet*) radio_buf);		// q points to radio_buf which is in packet format
               switch(q->type)
               {
                  case STATUS:
                     uart_println("\tCurrent Height: %u\r\n",q->payload.status.currHeight);
                     cState=CONTROLLER;
					 // set_standby_mode() is set at end of ISR
                     break;
                  case DEBUG:
                     uart_println("\t%s\r\n",q->payload.msg);
                     break;
                  case READY:
                     uart_println("\t%s\r\n",q->payload.msg);
                     break;
                  case CONTROL:
	              default:
   	                 uart_println("\tUnknown packet received.\r\n");
   		             break;
               }
               packet_available = 0;
            }
            else if (count==TIME_OUT)
            {
               cState = CONTROLLER;
			   count=0;
               set_standby_mode();
               uart_println("Time out has occurred.\r\n");
            } else
            	count++;
            break;
	     case(CONTROLLER):

            /* Set "data" being sent to blimp */
            p.type=CONTROL;
            p.payload.control.X++;
            p.payload.control.Y++;
            p.payload.control.Z++;
            radio_send(BLIMP_ADDRESS, (uint8_t*) &p);
            radio_set_receive();
			count=0;
            cState=LISTEN;
            break;
         default:
            uart_println("\tState not known.\r\n");
            return 0;
       }
       _delay_ms(POLL_INTERVAL); 
   }
   return 0;

} // end main function

