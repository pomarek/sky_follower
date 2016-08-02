#include <stddef.h>
#include "init.h"
#include "core.h"
#include "common_macros.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"
#include "stepper_drv.h"
#include "buttons.h"
#include "commands.h"
#include "debug.h"

//TODO LIST:
//1. uart control
//   - on/off
//   - speed/dir
//   - home
//   - debug control
//   - precise move
//2. endstop support
//3. angular speed
//4. reset log


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
    {
        char buf[20];
        int read;
        read = uart_iread(buf, 20);
        uart_isend(buf, read);
        handle_buttons();
        //commands_process();
    }
}

static void dir_button_handler(gpio_state_t state, stepper_dir_t dir)
{
    if( state == GPIO_STATE_LOW)
        stepper_start_ex(2000, 0, dir);
    else
        stepper_stop();
}

static void btn_up_handler(void)
{
    log_print(LOG_INFO, "btn up\r\n");
    dir_button_handler(buttons_get_state(BTN_UP), STEPPER_DIR_THIS);
}

static void btn_down_handler(void)
{
    log_print(LOG_INFO, "btn down\r\n");
    dir_button_handler(buttons_get_state(BTN_DOWN), STEPPER_DIR_OTHER);
}

static void btn_start_handler(void)
{
    uart_isend("aaa\r\n", 5);
}

void generic_btn_handler(gpio_t button_pin, gpio_state_t state)
{
    //this is interrupt so let's not make much here
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

bool set_speed_command(char * params)
{
    log_print(LOG_TRACE, "func %s, param %s\r\n", __FUNCTION__, params);
    return true;
}

bool start_command(char * params)
{
    log_print(LOG_TRACE, "func %s, param %s\r\n", __FUNCTION__, params);
    return true;
}

bool stop_command(char *params)
{
    log_print(LOG_TRACE, "func %s, param %s\r\n", __FUNCTION__, params);
    return true;
}
bool move_command(char *params)
{
    log_print(LOG_TRACE, "func %s, param %s\r\n", __FUNCTION__, params);
    return true;
}
bool home_command(char *params)
{
    log_print(LOG_TRACE, "func %s, param %s\r\n", __FUNCTION__, params);
    return true;
}

bool debug_level_command(char *params)
{
    log_print(LOG_TRACE, "func %s, param %s\r\n", __FUNCTION__, params);
    return true;
}

int main(void)
{
    system_init();
    per_init();
    debug_init(DEBUG_MODE_UART, LOG_DEBUG);
    buttons_enable(BTN_UP, generic_btn_handler);
    buttons_enable(BTN_DOWN, generic_btn_handler);
    buttons_enable(BTN_START, generic_btn_handler);
    stepper_on();
    forever_loop();
    system_reset();
    return 0;
}
