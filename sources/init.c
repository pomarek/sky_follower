#include "init.h"
#include "core.h"
#include "gpio.h"
#include "config.h"

void system_init(void)
{
    clock_init(CLOCK_SOURCE, CPU_SPEED);
}

void per_init(void)
{
    gpio_init(GP_EN, GPIO_DIRECTION_OUT);
    gpio_set_output_type(GP_EN, GPIO_OUTPUT_TYPE_PUSH_PULL);
    gpio_clear_pullup(GP_EN);
    
}
