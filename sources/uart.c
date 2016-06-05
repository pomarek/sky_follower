#include <stm32f0xx.h>

#include "common_macros.h"
#include "core.h"
#include "gpio.h"
#include "uart.h"

#define STOP_BITS_OFF 12

void uart_init(gpio_t tx_pin, gpio_t rx_pin, uint32_t baud, uart_data_bits_t data ,uart_parity_t parity, uart_stop_t stop_bits)
{
    uint32_t div;
    uint32_t reg = 0;
    
    power_on_device(DEVICE_ID_UART);
    div = SystemCoreClock/baud;
    mux_set(tx_pin, MUX_FUNC_UART);
    mux_set(rx_pin, MUX_FUNC_UART);
    
    USART1->CR1 &= ! USART_CR1_UE; // disable uart
    
    reg = USART1->CR1;
    if(data == UART_DATA_BITS_9)
    {
        reg |= USART_CR1_M;
    }
    else
    {
        reg &= ~USART_CR1_M;
    }
    
    switch(parity)
    {
        case UART_PARITY_NONE:
            reg &= ~USART_CR1_PCE;
            break;
        case UART_PARITY_EVEN:
            reg |= USART_CR1_PCE;
            reg &= ~USART_CR1_PS;
            break;
        case UART_PARITY_ODD:
            reg |= USART_CR1_PCE;
            reg |= USART_CR1_PS;
            break;
    }
    USART1->CR1 = reg;
    USART1->CR2 =  (stop_bits<<STOP_BITS_OFF);
    USART1->CR3 = 0;
    USART1->BRR = div;
    
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void uart_poll_send(char *data, uint32_t size)
{
    int i;
    for(i = 0; i<size; i++)
    {
        while((USART1->ISR & USART_ISR_TXE) == 0)
            ;
        USART1->TDR = ((uint32_t)(data[i])) & BYTE_MASK; //add support for 7 and 9 bit transmission
    }
}

uint32_t uart_pool_read(char *data, uint32_t size)
{
    int i;
    for(i=0; i<size && (USART1->ISR & USART_ISR_RXNE) ; i++)
    {
        data[i]= (char)(USART1->RDR & BYTE_MASK);
    }
    return 0;
}
