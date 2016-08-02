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
log_level_t lvl;

const char *debug_levels[] = {"CRITICAL: ", "ERROR: ", "WARNING: ", "INFO: ", "DEBUG: ", "TRACE: "};

void log_print(log_level_t loglvl, char * format, ...)
{
    static char buffer[MAX_PRINT_STR]; //static to make sure it does not use stack
    if(loglvl <= lvl)
    {
        if(dmode != DEBUG_MODE_NONE)
        {
            va_list args;
            va_start (args, format);
            vsnprintf (buffer, MAX_PRINT_STR, format, args);
            va_end (args);
            
            if(dmode == DEBUG_MODE_UART)
            {
                uart_poll_send((char*)debug_levels[loglvl], strlen(debug_levels[loglvl]));
                uart_poll_send(buffer, strlen(buffer));
            }
        }
    }
}

void unasigned_handler(IRQn_Type id) 
{
    uart_isend("unexpected interrupt %d\r\n", strlen("unexpected interrupt %d\r\n"));
}

void log_set_level(log_level_t loglvl)
{
    if(loglvl <= LOG_TRACE)
        lvl = loglvl;
}

static void error_int(IRQn_Type id, void * data)
{
    uart_isend("hard_fault\r\n", strlen("hard_fault\r\n"));
    //TODO: add detailed error desc
}

void debug_init(debug_mode_t mode, log_level_t loglvl)
{
    dmode = mode;
    log_set_level(loglvl);
    if(mode != DEBUG_MODE_NONE)
        interrupt_set_handler(HardFault_IRQn, error_int, NULL);
}
