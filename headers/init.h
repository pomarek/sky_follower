#ifndef _INIT_H_
    #define _INIT_H_

#include <stdbool.h>
#include <stdint.h>

void system_init(void);
void per_init(void);

//systick related stuff
typedef int (*timeout_handler_t)(uint64_t current_time, void * data);

void sleep(uint32_t msec);
int trigger_sleep(uint32_t msec, volatile uint32_t * trigger);
uint64_t get_uptime(void);
int register_timeout(timeout_handler_t handler, uint32_t timeout, void * data);
int unregister_timeout(int index);

#endif //_INIT_H_

