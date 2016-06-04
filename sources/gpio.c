#include <stddef.h>
#include <stm32f0xx.h>
#include "core.h"
#include "gpio.h"

#define PIN_RESET_BIT_OFFSET 16
#define GPIO_AHBENR_OFF 17

#define MUX_IS_GPIO(FUNC) ((MUX_MODER(FUNC) & 2) == 0)
#define MUX_IS_ALT(FUNC) (MUX_MODER(FUNC) == 2)
#define MUX_IS_ANALOG(FUNC) (MUX_MODER(FUNC) == 3)

#define MUX_MODER(FUNC) ((FUNC)>>4)
#define MUX_AFR(FUNC) ((FUNC)&0x0F)
#define MUX_DOUBLE_OFF(GPIO) (GPIO_PIN(GPIO)<<1)
#define MUX_QUAD_OFF(GPIO) ((GPIO_PIN(GPIO)<<2)&0x07)
#define MUX_IS_AFRH(GPIO) (GPIO_PIN(GPIO) > 7)

typedef enum
{
    GPIO_PULLUP_MODE_NONE = 0x00,
    GPIO_PULLUP_MODE_PULLUP = 0x01,
    GPIO_PULLUP_MODE_PULLDOWN = 0x02
}gpio_pullup_mode_t;

GPIO_TypeDef* banks[] = {GPIOA, GPIOB, GPIOC, GPIOD, NULL, GPIOF};
 
static GPIO_TypeDef * get_gpio_addr(gpio_t gpio)
{
    uint32_t bank = GPIO_BANK(gpio);
    if(bank < (sizeof(banks)/sizeof(banks[0])))
    {
        return banks[bank];
    }
    return NULL;
}

void mux_set(gpio_t gpio, mux_function_t func)
{
    GPIO_TypeDef* regs;
    regs = get_gpio_addr(gpio);
    
    if(regs)
    {
        uint32_t val, i =0;
        val = regs->MODER & (~(3 << MUX_DOUBLE_OFF(gpio)));
        val |= (MUX_MODER(func) << MUX_DOUBLE_OFF(gpio));
        regs->MODER = val;
        if(MUX_IS_ALT(func))
        {
            if(MUX_IS_AFRH(gpio))
            {
                i = 1;
            }
            val =  regs->AFR[i] & (~(15 << MUX_QUAD_OFF(gpio)));
            regs->AFR[i] = val | (MUX_AFR(func) << MUX_QUAD_OFF(gpio));
        }
    }
}

void gpio_set_speed(gpio_t gpio, gpio_speed_t speed)
{
    GPIO_TypeDef* regs;
    regs = get_gpio_addr(gpio);
    if(regs)
    {
        uint32_t val;
        val = regs->OSPEEDR;
        val &= ~ (3 << MUX_DOUBLE_OFF(gpio));
        regs->OSPEEDR = val | (speed << MUX_DOUBLE_OFF(gpio));
    }

}

void gpio_set_output_type(gpio_t gpio, gpio_output_type_t output_type)
{
    GPIO_TypeDef* regs;
    uint32_t pin;
    regs = get_gpio_addr(gpio);
    if(regs)
    {
        uint32_t val;
        pin = GPIO_PIN(gpio);
        val = regs->OTYPER;
        val &= ~(1 << pin);
        regs->OTYPER = val | (output_type << pin);
    }

}

void gpio_init(gpio_t gpio, gpio_direction_t dir)
{
    power_on_device((device_id_t)(GPIO_BANK(gpio) + DEVICE_ID_GPIO_A));
    if(dir == GPIO_DIRECTION_OUT)
    {
        mux_set(gpio, MUX_FUNC_GPIO_OUT);
    }
    else
    {
        mux_set(gpio, MUX_FUNC_GPIO_IN);
    }

}

void gpio_set_pullup(gpio_t gpio, gpio_state_t state)
{
    gpio_pullup_mode_t pullup;
    GPIO_TypeDef* regs;
    regs = get_gpio_addr(gpio);
    switch(state)
    {
        case GPIO_STATE_LOW:
            pullup = GPIO_PULLUP_MODE_PULLDOWN;
            break;
        case GPIO_STATE_HIGH:
            pullup = GPIO_PULLUP_MODE_PULLUP;
            break;
        default:
            pullup = GPIO_PULLUP_MODE_NONE;
            break;
    }
    if(regs)
    {
        uint32_t val;
        val = regs->PUPDR;
        val &= ~ (3 << MUX_DOUBLE_OFF(gpio));
        regs->PUPDR = val | (pullup << MUX_DOUBLE_OFF(gpio));
    }    
}
void gpio_clear_pullup(gpio_t gpio)
{
    GPIO_TypeDef* regs;
    regs = get_gpio_addr(gpio);
    if(regs)
    {
        regs->PUPDR &= ~(3 << MUX_DOUBLE_OFF(gpio));
    }
}

void gpio_set(gpio_t gpio, gpio_state_t state)
{
    GPIO_TypeDef* regs;
    uint32_t pin;
    regs = get_gpio_addr(gpio);
    if(regs)
    {
        pin = GPIO_PIN(gpio);
        if(state == GPIO_STATE_LOW)
        {
            pin += PIN_RESET_BIT_OFFSET;
        }
        regs->BSRR = (1<<pin);
    }
}

gpio_state_t gpio_get(gpio_t gpio)
{
    gpio_state_t result = GPIO_STATE_LOW;
    GPIO_TypeDef* regs;
    uint32_t pin;
    regs = get_gpio_addr(gpio);
    if(regs)
    {
        pin = GPIO_PIN(gpio);
        result = (regs->IDR &(1<<pin))?GPIO_STATE_HIGH:GPIO_STATE_LOW;
    }
    
    return result;
}
