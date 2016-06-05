#include "init.h"
#include "core.h"
#include "common_macros.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

#define TEXT "some test text\r\n"

void forever_loop()
{
    int i;
    while(1)
    {
        uart_poll_send(TEXT, sizeof(TEXT));
        for(i = 0; i < MHZ(4); i++)
            ;
    }
}

void periodic_int(IRQn_Type id, void * data)
{
    gpio_state_t *a = (gpio_state_t*)data;
    gpio_set(GP_EN, *a);
    *a = (*a == GPIO_STATE_LOW)?GPIO_STATE_HIGH:GPIO_STATE_LOW;
}

int main(void)
{
    gpio_state_t a = GPIO_STATE_LOW;
    system_init();
    per_init();
    init_systic_timer(333333, periodic_int, &a);
    forever_loop();
    system_reset();
    return 0;
}
