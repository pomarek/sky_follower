#ifndef _PWM_H_
    #define _PWM_H_
#include <stdint.h>

typedef enum
{
    STEPPER_DIR_THIS,
    STEPPER_DIR_OTHER //;)
}stepper_dir_t;

void stepper_init(void);
void stepper_start(uint32_t step_delay, stepper_dir_t dir);//NOTE: Only 16 bit precision will be used eg. 0x8FFF0000 = 0x8FFFDADA, 0x0080FF00 = 0x0080FF33
void stepper_start_ex(uint16_t step_delay, uint16_t clkdiv, stepper_dir_t dir);
void stepper_stop(void);
void stepper_update_delay(uint32_t step_delay);
void stepper_update_delay_ex(uint16_t step_delay, uint16_t clock_div);
void stepper_update_dir(stepper_dir_t dir);
void stepper_step(void);
void stepper_on(void);
void stepper_off(void);



#endif //_PWM_H_
