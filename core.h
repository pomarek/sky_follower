#ifndef _CORE_H_
    #define _CORE_H_

#include "common_macros.h"

#define INTERNAL_CLOCK MHZ(8)
#define EXTERNAL_CLOCK MHZ(8)


typedef enum
{
    CPU_SPEED_8MHZ = 0,
    CPU_SPEED_16MHZ,
    CPU_SPEED_24MHZ,
    CPU_SPEED_32MHZ,
    CPU_SPEED_40MHZ,
    CPU_SPEED_48MHZ
} cpu_speed_t;

typedef enum
{
    CLOCK_SOURCE_INTERNALRC,
    CLOCK_SOURCE_EXTERNAL
} clock_source_t;

int clock_init(clock_source_t source, cpu_speed_t desired_speed);
int flash_init(void);
void enable_clock(clock_source_t source);
void system_reset(void);



#endif //_CORE_H_
