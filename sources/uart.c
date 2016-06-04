#include <stm32f0xx.h>
#include "uart.h"

void uart_init(gpio_t tx_pin, gpio_t rx_pin, uart_baud_t baud, uart_parity_t parity, uart_stop_t stop_bits)
{
    
}

void uart_poll_send(uint8_t *data, uint32_t size)
{
}

uint32_t uart_pool_read(uint8_t *data, uint32_t size)
{
    return 0;
}
