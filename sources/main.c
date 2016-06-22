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


typedef enum
{
    BUTTON_FIRST = 0,
    BUTTON_UP = BUTTON_FIRST,
    BUTTON_DOWN,
    BUTTON_START,
    BUTTON_COUNT
} button_t;

typedef void (*button_handler)(void);
static void btn_up_handler(void);
static void btn_down_handler(void);
static void btn_start_handler(void);

uint32_t delay;

const button_handler button_handlers[BUTTON_COUNT] = {btn_up_handler, btn_down_handler, btn_start_handler};
uint32_t btn_status;

static void handle_buttons(void)
{
    button_t i;
    for(i = BUTTON_FIRST; i<BUTTON_COUNT; i++)
    {
        if(btn_status & BIT(i))
        {
            btn_status &= ~ BIT(i);
            button_handlers[i]();
        }
    }
}

void forever_loop()
{
    while(1)
        handle_buttons();
}
#define MIN(X, Y) ((X)>(Y)?(Y):(X))
#define MAX_DEL 12000
static uint32_t calc_delay(void)
{
    if(delay>MAX_DEL)
    {
        if(delay > (MAX_DEL<<1) -2)
            delay = (MAX_DEL<<1) -2;
        return (MAX_DEL<<1) - delay;
        
    }
    else if(delay <2)
    {
        return 2;
    }
    else if(delay<MAX_DEL)
    {
        return delay;
    }
    else
    {
        return MAX_DEL;
    }
}

static stepper_dir_t calc_dir(void)
{
    return delay>MAX_DEL?STEPPER_DIR_THIS:STEPPER_DIR_OTHER;
}

int step(void)
{
    uint32_t delay_tmp = calc_delay();
    if(delay_tmp >(MAX_DEL>>2))
    {
        return 1000;
    }
    if(delay_tmp >10)
    {
        return (delay_tmp>>2);
    }
    return 1;
}

static void btn_up_handler(void)
{
    uint32_t delay_tmp;
    stepper_dir_t dir = calc_dir();
    delay += step();
    delay_tmp = calc_delay();
    log_print("up %d \r\n", delay_tmp);
    
    if(dir != calc_dir())
        stepper_update_dir(calc_dir());
    stepper_update_delay(delay_tmp);
}

static void btn_down_handler(void)
{
    uint32_t delay_tmp;
    stepper_dir_t dir = calc_dir();
    delay -= step();
    delay_tmp = calc_delay();
    log_print("down %d \r\n", delay_tmp);
    
    if(dir != calc_dir())
        stepper_update_dir(calc_dir());
    stepper_update_delay(delay_tmp);

}

static void btn_start_handler(void)
{
    static int aaaa = 0;
    if(aaaa == 0)
    {
        delay = MAX_DEL;
        stepper_start(calc_delay(), calc_dir());
        aaaa = 1;
        log_print("start\r\n");
    }
    else
    {
        stepper_stop();
        aaaa = 0;
        log_print("stop\r\n");
    }
}

void generic_btn_handler(gpio_t button_pin, gpio_state_t state)
{
    //this is interrupt so let's not make much here
    if(state == GPIO_STATE_LOW)
    {
        switch(button_pin)
        {
            case BTN_DOWN:
                btn_status |= BIT(BUTTON_DOWN);
                break;
            case BTN_UP:
                btn_status |= BIT(BUTTON_UP);
                break;
            case BTN_START:
                btn_status |= BIT(BUTTON_START);
                break;
            default:
                break;
        }
    }
}

int main(void)
{
    system_init();
    per_init();
    debug_init(DEBUG_MODE_UART);
    buttons_enable(BTN_UP, generic_btn_handler);
    buttons_enable(BTN_DOWN, generic_btn_handler);
    buttons_enable(BTN_START, generic_btn_handler);
    stepper_on();
    forever_loop();
    system_reset();
    return 0;
}
