#include "gpio.h"
#include "stepper_drv.h"

#include "config.h"


void stepper_init()
{
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

void stepper_start(uint32_t step_delay, stepper_dir_t dir)
{

}

void stepper_stop()
{

}

void stepper_update_delay(uint32_t step_delay)
{

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
