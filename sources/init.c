#include <stddef.h>
#include "init.h"
#include "core.h"
#include "gpio.h"
#include "uart.h"
#include "stepper_drv.h"
#include "buttons.h"
#include "config.h"

#define HANDLERS_NUM 5

typedef struct
{
    uint64_t next_invoke;
    timeout_handler_t handler;
    void * data;
    uint32_t timeout;
}periodic_handler_t;

static volatile uint64_t current_time = 0;
static volatile periodic_handler_t handlers[HANDLERS_NUM];

int register_timeout(timeout_handler_t handler, uint32_t timeout, void * data)
{
    int i;
    
    if(handler == NULL || timeout == 0)
        return -1;
    
    for(i=0; i< HANDLERS_NUM; i++)
    {
        if(handlers[i].handler == NULL)
        {
            handlers[i].handler = handler;
            handlers[i].data = data;
            handlers[i].timeout = timeout;
            handlers[i].next_invoke = current_time + timeout;
        }
    }
    return -1;
}

int unregister_timeout(int index)
{
    if(index >= HANDLERS_NUM)
        return -1;
    if(handlers[index].handler)
    {
        handlers[index].handler = NULL;
        handlers[index].data = NULL;
        return 0;
    }
    return 1;
}

static void handle_timeouts()
{
    int i;
    for(i=0; i<HANDLERS_NUM; i++)
    {
        if(handlers[i].handler && handlers[i].next_invoke < current_time)
        {
            if(handlers[i].handler(current_time, handlers[i].data))
            {
                handlers[i].next_invoke = current_time + handlers[i].timeout;
            }
            else
            {
                handlers[i].handler = NULL;
                handlers[i].data = NULL;
            }
            
        }
    }
    
}
static void periodic_int(IRQn_Type id, void * data)
{
    current_time++;
    handle_timeouts();
}


void sleep(uint32_t msec)
{
    uint64_t timeout = current_time + ((msec)*1000/TICK_TIMEOUT);
    while(timeout >= current_time)
        ;
}

uint64_t get_uptime(void)
{
    return current_time;
}

void system_init(void)
{
    clock_init(CLOCK_SOURCE, CPU_SPEED);
}

void per_init(void)
{
    gpio_init(GP_EN, GPIO_DIRECTION_OUT);
    gpio_set_output_type(GP_EN, GPIO_OUTPUT_TYPE_PUSH_PULL);
    gpio_clear_pullup(GP_EN);
    
    init_systic_timer(TICK_TIMEOUT, periodic_int, NULL); // millisecond timeouts
    
    uart_init(TX_PIN, RX_PIN, BAUD, DATA_BITS, PARITY, STOP_BITS);
    stepper_init();
    buttons_init();
    
}
