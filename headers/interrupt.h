#ifndef _INTERRUPT_H_
    #define _INTERRUPT_H_

#include <stm32f0xx.h>

typedef void (*int_handler)(IRQn_Type id, void * data);

void interrupt_enable(IRQn_Type id);
void interrupt_disable(IRQn_Type id);
void interrupt_set_handler(IRQn_Type id, int_handler handler, void* data);

#endif //_INTERRUPT_H_
