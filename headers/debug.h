#ifndef _DEBUG_H_
    #define _DEBUG_H_

typedef enum
{
    DEBUG_MODE_NONE,
    DEBUG_MODE_UART
} debug_mode_t;

typedef enum
{
    LOG_CRIT,
    LOG_ERR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_TRACE
} log_level_t;

void debug_init(debug_mode_t mode, log_level_t loglvl);
void log_set_level(log_level_t loglvl);
void log_print(log_level_t loglvl, char * format, ...);

#endif //_DEBUG_H_
