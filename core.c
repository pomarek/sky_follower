#include <stdint.h>
#include <stm32f0xx.h>
#include "system_stm32f0xx.h"
#include "core.h"

#define PLLMUL_Offset 18
#define RCC_CFGR_SW_Offset 0

#define CLOCK_SOURCE_PLL 2



static uint32_t calc_pll(clock_source_t source, cpu_speed_t desired_speed)
{
    uint32_t pllmul;
    if(source == CLOCK_SOURCE_INTERNALRC)
    {
        pllmul=desired_speed << 1;
    }
    else
    {
        pllmul= desired_speed -1 ;
    }
    return (pllmul << PLLMUL_Offset) & RCC_CFGR_PLLMUL;

}

void enable_clock(clock_source_t source)
{
    uint32_t on, rdy;
    switch(source)
    {
        case CLOCK_SOURCE_EXTERNAL:
            on = RCC_CR_HSEON;
            rdy = RCC_CR_HSERDY;
            break;
        case CLOCK_SOURCE_INTERNALRC:
        default:
            on = RCC_CR_HSION;
            rdy = RCC_CR_HSIRDY;
            break;
    }
    //enable desired and wait for it
    RCC->CR |= on; // enable desired
    while((RCC->CR & rdy) == 0)
        ;
}

static void setup_pll(clock_source_t source, cpu_speed_t desired_speed)
{
    uint32_t pllmul;
    //disable pll and wait for it
    RCC->CR &= ~RCC_CR_PLLON;
    while(RCC->CR & RCC_CR_PLLRDY)
        ;
    
    if(desired_speed != CPU_SPEED_8MHZ)//if pll is needed
    {
        enable_clock(source);
        pllmul = calc_pll(source, desired_speed);
        RCC->CFGR |= pllmul;
        
        if(source == CLOCK_SOURCE_EXTERNAL)
        {
            
            RCC->CFGR |= RCC_CFGR_PLLSRC;
        }
        else
        {
            RCC->CFGR &= ~RCC_CFGR_PLLSRC;
        }
        //enable pll and wait for lock
        RCC->CR |= RCC_CR_PLLON;
        while((RCC->CR & RCC_CR_PLLRDY) == 0)
            ;
    }
}

static void clock_select(clock_source_t source, cpu_speed_t desired_speed)
{
    uint32_t reg_val;
    enable_clock(source);
    
    //switch to desired
    reg_val = RCC->CFGR & (~RCC_CFGR_SW);
    if(desired_speed != CPU_SPEED_8MHZ)
    {
        reg_val |= (CLOCK_SOURCE_PLL << RCC_CFGR_SW_Offset);
    }
    else
    {
        reg_val |= (desired_speed << RCC_CFGR_SW_Offset);
    }
    RCC->CFGR = reg_val;
    
    //do not disable other since peripherials can still use it
    
}

int clock_init(clock_source_t source, cpu_speed_t desired_speed)
{
    clock_select(CLOCK_SOURCE_INTERNALRC, CPU_SPEED_8MHZ); // switch to irc no pll (to allow pll updates)
    setup_pll(source, desired_speed);
    clock_select(source, desired_speed);
    SystemCoreClockUpdate();
    return 0;
}

int flash_init(void)
{
    return 0;
}

void system_reset(void)
{
    NVIC_SystemReset();
}


