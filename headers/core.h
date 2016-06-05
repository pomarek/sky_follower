#ifndef _CORE_H_
    #define _CORE_H_
#include <stdint.h>
#include "common_macros.h"
#include "interrupt.h"
#define INTERNAL_CLOCK MHZ(8)
#define EXTERNAL_CLOCK MHZ(8)


typedef enum
{
    CPU_SPEED_8MHZ = 0,
    CPU_SPEED_16MHZ,
    CPU_SPEED_24MHZ,
    CPU_SPEED_32MHZ,
    CPU_SPEED_40MHZ,
    CPU_SPEED_48MHZ
} cpu_speed_t;

typedef enum
{
    CLOCK_SOURCE_INTERNALRC,
    CLOCK_SOURCE_EXTERNAL
} clock_source_t;

//0xXXYYZZ - XX - has reset bit, YY - bus register, ZZ - index in bus register
typedef enum
{
    //AHB
    DEVICE_ID_DMA       = 0x000000,
    DEVICE_ID_CRC       = 0x000006,
    DEVICE_ID_GPIO_A    = 0x010011,
    DEVICE_ID_GPIO_B    = 0x010012,
    //DEVICE_ID_GPIO_C    = 0x010013,
//    DEVICE_ID_GPIO_D    = 0x010014,
    DEVICE_ID_GPIO_F    = 0x010016,
    
    //APB2
    DEVICE_ID_SYSCFG    = 0x0100,
    DEVICE_ID_ADC       = 0x0109,
    DEVICE_ID_TIM1      = 0x010B,
    DEVICE_ID_SPI       = 0x010C,
    DEVICE_ID_UART      = 0x010E,
    DEVICE_ID_TIM15     = 0x0110,
    DEVICE_ID_TIM16     = 0x0111,
    DEVICE_ID_TIM17     = 0x0112,
    DEVICE_ID_DBGMCU    = 0x0116,
    
    //APB1
    DEVICE_ID_TIM3      = 0x0201,
    DEVICE_ID_TIM6      = 0x0204,
    DEVICE_ID_TIM7      = 0x0205,
    DEVICE_ID_TIM14     = 0x0208,
    DEVICE_ID_WWDG      = 0x020B,
    DEVICE_ID_I2C       = 0x0215,
    DEVICE_ID_PWRIF     = 0x021C
}device_id_t;

int clock_init(clock_source_t source, cpu_speed_t desired_speed);
int flash_init(void);
void enable_clock(clock_source_t source);
void system_reset(void);
void power_on_device(device_id_t dev);
void power_off_device(device_id_t dev);
//WARNING:
//if SystemCoreClock/1000000 * usec_timeout is greater than 0x00FFFFFF (16 777 215) error will be returned
//in case longer intervals are needed handler needs to count  how many times it was called
//MAX TIMEOUTS
//@48MHz - 0.349s max interval 
//@40MHz - 0.419s max interval 
//@32MHz - 0.524s max interval 
//@24MHz - 0.699s max interval 
//@16MHz - 1.048s max interval 
//@ 8MHz - 2.097s max interval 
uint32_t init_systic_timer(uint32_t interval, int_handler handler, void* custom_data);
void disable_systic_timer(void);





#endif //_CORE_H_
