
#ifndef _UART_H_
    #define _UART_H_
#include <stdint.h>
#include "gpio.h"

//NOTE: I'm to lazy to implement flow control

typedef enum
{
    UART_BAUD_9600,
    UART_BAUD_115200,
}uart_baud_t;

typedef enum
{
    UART_PARITY_NONE,
    UART_PARITY_ODD,
    UART_PARITY_EVEN
}uart_parity_t;

void uart_init(gpio_t tx_pin, gpio_t rx_pin, uart_baud_t baud, uart_parity_t parity);
void uart_poll_send(uint8_t *data, uint32_t size);
uint32_t uart_pool_read(uint8_t *data, uint32_t size);
#endif //_UART_H_
