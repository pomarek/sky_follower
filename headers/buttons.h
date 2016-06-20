#ifndef _BUTTONS_H_
    #define _BUTTONS_H_

#include <gpio.h>

typedef void (*button_handler_t)(gpio_t button_pin, gpio_state_t state);

void buttons_init(void);
void buttons_enable(gpio_t button_pin, button_handler_t handler);
void buttons_disable(gpio_t button_pin);
    
#endif //_BUTTONS_H_
