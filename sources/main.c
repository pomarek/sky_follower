#include "init.h"
#include "core.h"
#include "common_macros.h"
#include "config.h"
#include "gpio.h"
#include "uart.h"

#define TEXT "some test text\r\n"

void forever_loop()
{
    gpio_state_t a = GPIO_STATE_LOW;
    int i;
    while(1)
    {
        gpio_set(GP_EN, a);
        uart_poll_send(TEXT, sizeof(TEXT));
        a = (a == GPIO_STATE_LOW)?GPIO_STATE_HIGH:GPIO_STATE_LOW;
        for(i = 0; i < MHZ(4); i++)
            ;
    }
}

int main(void)
{
    system_init();
    per_init();
    forever_loop();
    system_reset();
    return 0;
}
