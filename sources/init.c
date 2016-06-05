#include "init.h"
#include "core.h"
#include "gpio.h"
#include "uart.h"
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
    uart_init(TX_PIN, RX_PIN, BAUD, DATA_BITS, PARITY, STOP_BITS);
}
