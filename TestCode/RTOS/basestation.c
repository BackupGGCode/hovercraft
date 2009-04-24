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
const unsigned int PT = 2;
const unsigned char PPP[] = {IDLE, 10, B, 100};
volatile uint8_t radio_buffer[PAYLOAD_BYTES];

int amount = 0;
char toPrint[255]; 

extern uint16_t trace_counter;
EVENT* radio_event;
EVENT* print_event;

void 
printInfo(infoPacket_t theInfo) 
{
    amount = sprintf(toPrint, SONAR_INFO_STRING, theInfo.frontSonar, theInfo.rightSonar,
                     theInfo.leftSonar);
    uart_write((uint8_t *)toPrint, amount);
    amount = sprintf(toPrint, MOTOR_INFO_STRING, theInfo.rightMotor, theInfo.leftMotor);
    uart_write((uint8_t *)toPrint, amount);
}

void uart_task(void)
{
   
    int arg = 0;
   
    arg = Task_GetArg();

    for(;;)
    {
        Event_Wait(radio_event);
        amount = sprintf(toPrint, "Inside the uart task\r\n");
        uart_write((uint8_t*) toPrint, amount);
        Task_Next();
    }
}



void event_task(void) {
    for(;;){
        Event_Signal(radio_event);
        Task_Next();
        amount = sprintf(toPrint, "Inside the event trigger task\r\n");
        uart_write((uint8_t*) toPrint, amount);
        Task_Next();
        amount = sprintf(toPrint, "Inside the event trigger task\r\n");
        uart_write((uint8_t*) toPrint, amount);
        Task_Next();
        amount = sprintf(toPrint, "Inside the event trigger task\r\n");
        uart_write((uint8_t*) toPrint, amount);
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
    print_event = Event_Init();

    Task_Create(uart_task, 1, RR, A);
    Task_Create(event_task, 2, PERIODIC, B);
//    Task_Create(generic_task, 3, RR, C);
//    Task_Create(generic_task, 4, RR, D);
//    Task_Create(generic_task, 5, RR, E);
//    Task_Create(generic_task, 6, RR, F);
    
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
    infoPacket_t *info;
    switch(incomingPacket->type) {
        case INFO:
            info = (infoPacket_t *)radio_buffer;
            printInfo(*info);
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

