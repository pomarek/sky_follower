#include <stddef.h>
#include "init.h"
#include "core.h"
#include "common_macros.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"
#include "stepper_drv.h"
#include "buttons.h"
#include "debug.h"

#define TEXT "some test text\r\n"
extern int run;

void forever_loop()
{
    while(1)
    {
        log_print(TEXT);
        sleep(5000);
    }
}

void btn_up_handler(gpio_t button_pin, gpio_state_t state)
{
    if(state == GPIO_STATE_LOW)
    {
        stepper_update_dir(STEPPER_DIR_THIS);
    }
}

void btn_down_handler(gpio_t button_pin, gpio_state_t state)
{
    if(state == GPIO_STATE_LOW)
    {
        stepper_update_dir(STEPPER_DIR_OTHER);
    }
}

void btn_start_handler(gpio_t button_pin, gpio_state_t state)
{
    static int aaaa = 0;
    if(state == GPIO_STATE_LOW)
    {
        if(aaaa == 0)
        {
            stepper_on();
            aaaa = 1;
        }
        else
        {
            stepper_off();
            aaaa = 0;
        }
    }
}

int main(void)
{
    system_init();
    per_init();
    debug_init(DEBUG_MODE_UART);
    buttons_enable(BTN_UP, btn_up_handler);
    buttons_enable(BTN_DOWN, btn_down_handler);
    buttons_enable(BTN_START, btn_start_handler);
    forever_loop();
    system_reset();
    return 0;
}
