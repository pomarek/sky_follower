#ifndef _INIT_H_
    #define _INIT_H_

#include <stdbool.h>
#include <stdint.h>

typedef int (*timeout_handler_t)(uint64_t current_time, void * data);

int register_timeout(timeout_handler_t handler, uint32_t timeout, void * data);
int unregister_timeout(int index);
void system_init(void);
void per_init(void);
void sleep(uint32_t msec);
uint64_t get_uptime(void);

#endif //_INIT_H_

