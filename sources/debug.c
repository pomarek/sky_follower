#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "interrupt.h"
#include "uart.h"
#include "debug.h"

#define MAX_PRINT_STR 200


debug_mode_t dmode;

void log_print(char * format, ...)
{
    static char buffer[MAX_PRINT_STR]; //static to make sure it does not use stack
    if(dmode != DEBUG_MODE_NONE)
    {
        va_list args;
        va_start (args, format);
        vsnprintf (buffer, MAX_PRINT_STR, format, args);
        va_end (args);
        
        if(dmode == DEBUG_MODE_UART)
            uart_poll_send(buffer, strlen(buffer));
    }
}

static void error_int(IRQn_Type id, void * data)
{
    log_print("hard_fault\r\n");
    //TODO: add detailed error desc
}

void debug_init(debug_mode_t mode)
{
    dmode = mode;
    if(mode != DEBUG_MODE_NONE)
        interrupt_set_handler(HardFault_IRQn, error_int, NULL);
}
