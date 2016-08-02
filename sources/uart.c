#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stm32f0xx.h>

#include "common_macros.h"
#include "core.h"
#include "gpio.h"
#include "uart.h"
#include "interrupt.h"

#define STOP_BITS_OFF 12
#define BUF_SIZE 0x20
#define BUF_MASK (BUF_SIZE -1)

volatile char inbuf[BUF_SIZE];
volatile uint8_t head =1, tail =0;
volatile static uint8_t data_sent;
volatile uart_senddata_t * outbuf;


void queue_append(char data)
{
    if(((head+1)&BUF_MASK) != tail)
    {
        inbuf[head] = data;
        head = (head +1)&BUF_MASK;
    }
}

uint32_t queue_get(char **buf)
{
    uint8_t tmp_head;
    if(head == tail)
    {
        *buf = NULL;
        return 0;
    }
    *buf = (char *)&inbuf[tail];
    tmp_head = head;
    if(tmp_head > tail)
    {
        return tmp_head - tail;
    }
    else
    {
        return BUF_SIZE - tail -1;
    }
}

int16_t queue_take(void)
{
    int16_t result;
    if(head == tail)
    {
        return -1;
    }
    result = inbuf[tail];
    tail = (tail +1)&BUF_MASK;
    return result;
}

void queue_remove(uint32_t size)
{
    tail = (tail + size) & BUF_MASK;
}


static void uart_int(IRQn_Type type, void * data)
{
    if(USART1->ISR & USART_ISR_RXNE)//data received
    {
        queue_append((char)(USART1->RDR));
    }
    while(USART1->ISR & USART_ISR_TXE) // space in send buffer available
    {
        if(outbuf)
        {
            if(outbuf->offset < outbuf->data_size)
            {
                USART1->TDR = ((uint32_t)(outbuf->data_buffer[outbuf->offset++])) & BYTE_MASK;
            }
            else
            {
                uart_senddata_t * buf = (uart_senddata_t *)outbuf;
                outbuf = buf->next;
                buf->handler(buf);
            }
        }
        else
        {
            USART1->CR1 &= ~USART_CR1_TXEIE;
            break;
        }
    }
}

static uint32_t prepare_CR1_data(uart_data_bits_t data, uart_parity_t parity)
{
    uint32_t reg = 0;
    
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
    reg |= USART_CR1_TE | /*USART_CR1_RXNEIE | */USART_CR1_RE;
    return reg ;
}

void uart_init(gpio_t tx_pin, gpio_t rx_pin, uint32_t baud, uart_data_bits_t data ,uart_parity_t parity, uart_stop_t stop_bits)
{
    power_on_device(DEVICE_ID_UART);
    mux_set(tx_pin, MUX_FUNC_UART);
    gpio_set_pullup(rx_pin, GPIO_STATE_HIGH);
    //mux_set(rx_pin, MUX_FUNC_GPIO_IN);
    mux_set(rx_pin, MUX_FUNC_UART);
    
    USART1->CR1 = 0; // disable uart
    
    USART1->CR1 = USART_CR1_TE|USART_CR1_RE;//prepare_CR1_data(data, parity);
    USART1->CR2 = 0;// (stop_bits<<STOP_BITS_OFF);
    USART1->CR3 = 0;
    USART1->BRR = SystemCoreClock/baud;
    
//    interrupt_set_handler(USART1_IRQn, uart_int, NULL);
//    interrupt_enable(USART1_IRQn);
    USART1->CR1 |= USART_CR1_UE;
}


//make this thread safe; unless done it can not be called elsewhere than from uart_poll***
void uart_send(uart_senddata_t * data)
{
    volatile uart_senddata_t * space;
    if(outbuf == NULL)
    {
        outbuf = data;
    }
    else
    {
        for(space = outbuf; space->next != NULL; space = space->next)
            ;
        space->next = data;
    }
    USART1->CR1 |= USART_CR1_TXEIE;
}

static void pool_transmission_complete(struct _uart_senddata_t * uart_data)
{
    data_sent = 1;
}

void uart_poll_send(char *data, uint32_t size)
{
    uart_senddata_t send_data;
    send_data.data_buffer = data;
    send_data.data_size = size;
    send_data.handler = pool_transmission_complete;
    send_data.next = NULL;
    send_data.offset = 0;
    
    data_sent = 0;
    uart_send(&send_data);
    while(data_sent == 0)
        ;
}
int16_t uart_read_byte(bool wait)
{
    int16_t read;
    do
    {
        read = queue_take();
    }while(wait == true && read == -1);
    return read;
}

uint32_t uart_pool_read(char *data, uint32_t size)
{
    int16_t read, read_size;
    for(read = queue_take(), read_size = 0; read != -1 && read_size<size; read = queue_take(), read_size++)
    {
        *(data++) = (char)read;
    }
    
    return read_size;
}

void uart_isend(char *data, uint32_t size)
{
    int i;
    for(i = 0; i<size; i++)
    {
        while((USART1->ISR & USART_ISR_TXE) == 0)
            ;
        USART1->TDR = ((uint32_t)(data[i])) & BYTE_MASK; //add support for 7 and 9 bit transmission
    }
}

uint32_t uart_iread(char *data, uint32_t size)
{
    int i;
    for(i=0; i<size && (USART1->ISR & USART_ISR_RXNE) ; i++)
    {
        data[i]= (char)(USART1->RDR & BYTE_MASK);
    }
    return i;
}


