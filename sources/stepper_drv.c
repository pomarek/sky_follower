#include <stm32f0xx.h>
#include "core.h"
#include "gpio.h"
#include "stepper_drv.h"

#include "config.h"
 
 #define PRESCALER 2000

void stepper_init()
{
    power_on_device(DEVICE_ID_TIM3);
    gpio_init(GP_EN, GPIO_DIRECTION_OUT);
    gpio_clear_pullup(GP_EN);
    gpio_set(GP_EN, GPIO_STATE_HIGH);
    gpio_init(GP_DIR1, GPIO_DIRECTION_OUT);
    gpio_init(GP_STEP1, GPIO_DIRECTION_OUT);
    gpio_clear_pullup(GP_DIR1);
    gpio_clear_pullup(GP_STEP1);
    gpio_set(GP_DIR1, GPIO_STATE_LOW);
    gpio_set(GP_STEP1, GPIO_STATE_LOW);

}

static void timer_config(uint16_t prescaler)
{
    TIM3->CCMR2 = TIM_CCMR2_OC4M_0|TIM_CCMR2_OC4M_1;
    TIM3->CCER = TIM_CCER_CC4E;
    TIM3->PSC = prescaler;
}

void stepper_start(uint32_t step_delay, stepper_dir_t dir)
{
    uint16_t actual_delay = step_delay &0x0000FFFF;
    mux_set(GP_STEP1, MUX_FUNC_TIM3);
    timer_config(PRESCALER);
    TIM3->ARR = actual_delay;
    TIM3->CCR4 = actual_delay-1;
    TIM3->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
}

void stepper_stop()
{
    TIM3->CR1 &= ~TIM_CR1_CEN;
    gpio_init(GP_STEP1, GPIO_DIRECTION_OUT);
}

void stepper_update_delay(uint32_t step_delay)
{
    uint16_t actual_delay = step_delay &0x0000FFFF;
    TIM3->ARR = actual_delay;
    TIM3->CCR4 = 0;
}

void stepper_update_dir(stepper_dir_t dir)
{

    if(dir == STEPPER_DIR_THIS)
    {
        gpio_set(GP_DIR1, GPIO_STATE_LOW);
    }
    else
    {
        gpio_set(GP_DIR1, GPIO_STATE_HIGH);
    }
}

void stepper_step()
{
    gpio_set(GP_STEP1, 
             (gpio_get(GP_STEP1) == GPIO_STATE_LOW)?GPIO_STATE_HIGH:GPIO_STATE_LOW);

}

void stepper_on(void)
{
    gpio_set(GP_EN, GPIO_STATE_LOW);
}

void stepper_off(void)
{
    gpio_set(GP_EN, GPIO_STATE_HIGH);
}
