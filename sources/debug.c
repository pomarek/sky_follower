#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "interrupt.h"
#include "uart.h"
#include "debug.h"


debug_mode_t dmode;

static void log_print(char * str)
{
    if(dmode == DEBUG_MODE_UART)
        uart_poll_send(str, strlen(str));
}

static void error_int(IRQn_Type id, void * data)
{
    log_print("hard_fault\r\n");
}

void debug_init(debug_mode_t mode)
{
    dmode = mode;
    if(mode != DEBUG_MODE_NONE)
        interrupt_set_handler(HardFault_IRQn, error_int, NULL);
}
