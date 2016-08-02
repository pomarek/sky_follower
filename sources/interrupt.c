#include <stddef.h>
#include "interrupt.h"

#define INTERRUPT_LAST USART1_IRQn
#define INTERRUPT_FIRST NonMaskableInt_IRQn
#define INTERRUPT_MAX (USART1_IRQn - NonMaskableInt_IRQn)
#define INTERRUPT_GET_ARRAY_INDEX(INT) ((INT) - NonMaskableInt_IRQn)

static int_handler handlers[INTERRUPT_MAX];
static void* data_list[INTERRUPT_MAX];

__weak void unasigned_handler(IRQn_Type id) 
{
}

void interrupt_enable(IRQn_Type id)
{
    NVIC_EnableIRQ(id);
}

void interrupt_disable(IRQn_Type id)
{
    NVIC_DisableIRQ(id);
}

void interrupt_set_handler(IRQn_Type id, int_handler handler, void* data)
{
    uint32_t index = INTERRUPT_GET_ARRAY_INDEX(id);
    if(id > INTERRUPT_LAST)
        return;
    handlers[index]= handler;
    data_list[index] = data;
}



static void DefaultIntHandler(IRQn_Type id)
{
    uint32_t index = INTERRUPT_GET_ARRAY_INDEX(id);
    if(handlers[index] != NULL)
    {
        handlers[index](id, data_list[index]);
    }
    else
    {
        unasigned_handler(id);
        while(1)
            ;
    }
}
//cpu int handlers
void  NMI_Handler(void)
{
    DefaultIntHandler(NonMaskableInt_IRQn);
}
void  HardFault_Handler(void)
{
    DefaultIntHandler(HardFault_IRQn);
}
void  SVC_Handler(void)
{
    DefaultIntHandler(SVC_IRQn);
}
void  PendSV_Handler(void)
{
    DefaultIntHandler(PendSV_IRQn);
}
void  SysTick_Handler(void)
{
    DefaultIntHandler(SysTick_IRQn);
}
void  WWDG_IRQHandler(void)
{
    DefaultIntHandler(WWDG_IRQn);
}
void  RTC_IRQHandler(void)
{
    DefaultIntHandler(RTC_IRQn);
}
void  FLASH_IRQHandler(void)
{
    DefaultIntHandler(FLASH_IRQn);
}
void  RCC_IRQHandler(void)
{
    DefaultIntHandler(RCC_IRQn);
}
void  EXTI0_1_IRQHandler(void)
{
    DefaultIntHandler(EXTI0_1_IRQn);
}
void  EXTI2_3_IRQHandler(void)
{
    DefaultIntHandler(EXTI2_3_IRQn);
}
void  EXTI4_15_IRQHandler(void)
{
    DefaultIntHandler(EXTI4_15_IRQn);
}
void  DMA1_Channel1_IRQHandler(void)
{
    DefaultIntHandler(DMA1_Channel1_IRQn);
}
void  DMA1_Channel2_3_IRQHandler(void)
{
    DefaultIntHandler(DMA1_Channel2_3_IRQn);
}
void  DMA1_Channel4_5_IRQHandler(void)
{
    DefaultIntHandler(DMA1_Channel4_5_IRQn);
}
void  ADC1_IRQHandler(void)
{
    DefaultIntHandler(ADC1_IRQn);
}
void  TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    DefaultIntHandler(TIM1_BRK_UP_TRG_COM_IRQn);
}
void  TIM1_CC_IRQHandler(void)
{
    DefaultIntHandler(TIM1_CC_IRQn);
}
void  TIM3_IRQHandler(void)
{
    DefaultIntHandler(TIM3_IRQn);
}
void  TIM14_IRQHandler(void)
{
    DefaultIntHandler(TIM14_IRQn);
}
void  TIM16_IRQHandler(void)
{
    DefaultIntHandler(TIM16_IRQn);
}
void  TIM17_IRQHandler(void)
{
    DefaultIntHandler(TIM17_IRQn);
}
void  I2C1_IRQHandler(void)
{
    DefaultIntHandler(I2C1_IRQn);
}
void  SPI1_IRQHandler(void)
{
    DefaultIntHandler(SPI1_IRQn);
}
void USART1_IRQHandler(void)
{
    DefaultIntHandler(USART1_IRQn);
}
