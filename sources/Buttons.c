#include <stddef.h>
#include <stdint.h>
#include <stm32f0xx.h>
#include "init.h"
#include "interrupt.h"
#include "buttons.h"
#include "common_macros.h"

//since no filters are available for gpio butons are handled by pooling

#define EXTICR_MASK 0x0F
#define BTN_TIMEOUT 100

#define BTN_REPEAT_CT 2

typedef struct
{
    uint8_t last_state:1;
    uint8_t counter:7;
}button_state_t;
    
static button_handler_t handlers[GP_MAX_COUNT];
static button_state_t states[GP_MAX_COUNT];



static int scan_buttons(uint64_t time, void* data)
{
    gpio_t i;
    for(i = (gpio_t)0; i<GP_MAX_COUNT; i++)
    {
        if(handlers[i])
        {
            gpio_state_t st = gpio_get(i);
            
            if(st == states[i].last_state)
            {
                if(states[i].counter <= BTN_REPEAT_CT)
                    states[i].counter++;
                
                if(BTN_REPEAT_CT == states[i].counter)
                    handlers[i](i, st);
            }
            else
            {
                states[i].counter = 0;
                states[i].last_state = st;
            }
        }
    }
    return 1;
}
void buttons_init(void)
{
    register_timeout(scan_buttons, BTN_TIMEOUT, NULL);
}


void buttons_enable(gpio_t button_pin, button_handler_t handler)
{
    if(handler)
    {
        mux_set(button_pin, MUX_FUNC_GPIO_IN);
        gpio_set_pullup(button_pin, GPIO_STATE_HIGH);
        states[button_pin].last_state = gpio_get(button_pin);
        states[button_pin].counter = BTN_REPEAT_CT + 1;
        handlers[button_pin] = handler;
        
    }
    else
    {
        buttons_disable(button_pin);
    }
}

void buttons_disable(gpio_t button_pin)
{
    handlers[button_pin] = NULL;
    
}
