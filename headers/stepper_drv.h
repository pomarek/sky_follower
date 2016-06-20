#ifndef _PWM_H_
    #define _PWM_H_
#include <stdint.h>

typedef enum
{
    STEPPER_DIR_THIS,
    STEPPER_DIR_OTHER //;)
}stepper_dir_t;

void stepper_init(void);
void stepper_start(uint32_t step_delay, stepper_dir_t dir);
void stepper_stop(void);
void stepper_update_delay(uint32_t step_delay);
void stepper_update_dir(stepper_dir_t dir);
void stepper_step(void);
void stepper_on(void);
void stepper_off(void);



#endif //_PWM_H_
