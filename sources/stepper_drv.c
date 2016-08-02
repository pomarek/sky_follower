#include <stm32f0xx.h>
#include "core.h"
#include "gpio.h"
#include "stepper_drv.h"
#include "debug.h"

#include "config.h"
 

static void timer_config(void);

void stepper_init()
{
    power_on_device(DEVICE_ID_TIM3);
    timer_config();
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

static void timer_config(void)
{
    TIM3->CCMR2 = TIM_CCMR2_OC4M_0|TIM_CCMR2_OC4M_1;
    TIM3->CCER = TIM_CCER_CC4E;
    TIM3->CCR4 = 0;
}

void stepper_start_ex(uint16_t step_delay, uint16_t clock_div, stepper_dir_t dir)
{
    mux_set(GP_STEP1, MUX_FUNC_TIM3);
    stepper_update_dir(dir);
    TIM3->ARR = step_delay;
    TIM3->PSC = clock_div;
    TIM3->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;

}
static void calc_clk(uint32_t step_delay, uint16_t * out_div, uint16_t * out_delay)
{
    int i;
    uint16_t div = 0xFFFF;
    
    for(i=31; i>15; i--)
    {
        if(BIT(i) & step_delay)
            break;
        div >>= 1;
    }
    
    *out_div = div;
    *out_delay = (step_delay >>(i - 15))&0x0000FFFF;
    log_print(LOG_DEBUG, "prescaler = %d, counter = %d\r\n", div, *out_delay);
}
void stepper_start(uint32_t step_delay, stepper_dir_t dir)
{
    uint16_t delay, div;
    calc_clk(step_delay, &div, &delay);
    stepper_start_ex(delay, div, dir);
}

void stepper_stop()
{
    TIM3->CR1 &= ~TIM_CR1_CEN;
    gpio_init(GP_STEP1, GPIO_DIRECTION_OUT);
}

void stepper_update_delay_ex(uint16_t step_delay, uint16_t clock_div)
{
    TIM3->ARR = step_delay;
    TIM3->PSC = clock_div;
}

void stepper_update_delay(uint32_t step_delay)
{
    uint16_t delay, div;
    calc_clk(step_delay, &div, &delay);
    stepper_update_delay_ex(delay, div);
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
