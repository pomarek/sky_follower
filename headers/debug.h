#ifndef _DEBUG_H_
    #define _DEBUG_H_

typedef enum
{
    DEBUG_MODE_NONE,
    DEBUG_MODE_UART
} debug_mode_t;

void debug_init(debug_mode_t mode);

#endif //_DEBUG_H_
