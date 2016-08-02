#ifndef _UART_H_
    #define _UART_H_
    
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

//NOTE: I'm to lazy to implement flow control

typedef enum
{
    UART_PARITY_NONE,
    UART_PARITY_ODD,
    UART_PARITY_EVEN
}uart_parity_t;

typedef enum
{
    UART_DATA_BITS_8 = 0x00,
    UART_DATA_BITS_9 = 0x01
}uart_data_bits_t;

typedef enum
{
    UART_STOP_1BIT = 0,
    UART_STOP_2BITS = 2,
    UART_STOP_1_5BIT = 3,
} uart_stop_t;

struct _uart_senddata_t;

typedef void (*transmission_complete_handler_t)(struct _uart_senddata_t * uart_data);

typedef struct _uart_senddata_t
{
    char * data_buffer;
    transmission_complete_handler_t handler;
    uint16_t data_size;
    uint16_t offset;
    struct _uart_senddata_t * next;
}uart_senddata_t;

void uart_init(gpio_t tx_pin, gpio_t rx_pin, uint32_t baud, uart_data_bits_t data, uart_parity_t parity, uart_stop_t stop_bits);
void uart_poll_send(char *data, uint32_t size);
uint32_t uart_pool_read(char *data, uint32_t size);
int16_t uart_read_byte(bool wait);

void uart_isend(char *data, uint32_t size);
uint32_t uart_iread(char *data, uint32_t size);
#endif //_UART_H_
