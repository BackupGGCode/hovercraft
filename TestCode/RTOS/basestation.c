/**
 * @file   basestation.c
 *
 * @brief  Basestation.
 */

#include "common.h"
#include "OS/os.h"
#include "radio/radio.h"
#include "uart/uart.h"
#include "trace/trace.h"

#define SONAR_INFO_STRING "Front sonar: %d, right sonar: %d, left sonar: %d\r\n"
#define MOTOR_INFO_STRING "Right motor: %d, lett motor: %d\r\n"

enum { A=1, B};
const unsigned int PT = 0;
const unsigned char PPP[] = {};
volatile uint8_t radio_buffer[PAYLOAD_BYTES];
infoPacket_t *info;

int amount = 0;
char toPrint[255]; 


EVENT* radio_event;


void 
printInfo(infoPacket_t theInfo) 
{
    amount = sprintf(toPrint, SONAR_INFO_STRING, theInfo.frontSonar, theInfo.rightSonar,
                     theInfo.leftSonar);
    uart_write((uint8_t *)toPrint, amount);
    amount = sprintf(toPrint, MOTOR_INFO_STRING, theInfo.rightMotor, theInfo.leftMotor);
    uart_write((uint8_t *)toPrint, amount);
}

void 

uart_task(void)
{
     for(;;)
    {
        Event_Wait(radio_event);
      	printInfo(*info);
        Task_Next();
    }
}







int main(void)
{
    cli();
    /* setup the test */
	uart_init();
	// uart_write((uint8_t*)"START\r\n", 9);
    // set_test(3);
    sei();

    radio_event = Event_Init();


    Task_Create(uart_task, 1, RR, A);
    Task_Create(event_task, 2, PERIODIC, B);

    
    return 0;
}

ISR (INT4_vect)
{
    int i;
    
    PORTD ^= _BV(PORTD5);
    
    for (i = 0; i < PAYLOAD_BYTES; i++)
    {
        radio_buffer[i] = radio_get_byte();
    }
    
    // Figure out what type of packet this is.
    genericPacket_t *incomingPacket = (genericPacket_t *)radio_buffer;
    
    switch(incomingPacket->type) {
        case INFO:
            info = (infoPacket_t *)radio_buffer;
            Event_Signal(radio_event);
			
            break;
        default:
            // ignore
            break;
    }
    
    /* print out the radio packet on uart */
    //uart_write((uint8_t*)radio_buffer, PAYLOAD_BYTES);
    
    /* setup the radio to receive another packet */
    radio_set_receive();
}

