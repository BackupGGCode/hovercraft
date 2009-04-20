/**
 * @file   test003.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 *
 * @brief  Test 003 - can schedule RR tasks in the expected order
 *
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B};
const unsigned int PT = 2;
const unsigned char PPP[] = {IDLE, 10, B, 100};

 int amount = 0;
 char toPrint[250]; 

extern uint16_t trace_counter;
EVENT* radio_event;
EVENT* print_event;

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



void event_task(void){
    
    
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
    /* setup the test */
	uart_init();
	// uart_write((uint8_t*)"START\r\n", 9);
    // set_test(3);

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
