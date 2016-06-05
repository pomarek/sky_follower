#ifndef _GPIO_H_
    #define _GPIO_H_

#define GPIO_BANK(GPIO) ((GPIO)>>8)
#define GPIO_PIN(GPIO) ((GPIO)&0x00FF)
#define GPIO_GPIO(BANK, PIN) (((BANK)<<8) +(PIN))

typedef enum
{
    GP_A0 = GPIO_GPIO(0, 0),//UART_CTS, ADC_IN0, RTC_TAMP2, WKUP1
    GP_A1 = GPIO_GPIO(0, 1),//UART_RTS, EVENTOUT, ADC_IN1
    GP_A2 = GPIO_GPIO(0, 2),//UART_TX, ADC_IN2
    GP_A3 = GPIO_GPIO(0, 3),//UART_RX, ADC_IN3
    GP_A4 = GPIO_GPIO(0, 4),//SPI_NNS, UART_CK, TIM14_CH1, ADC_IN4
    GP_A5 = GPIO_GPIO(0, 5),//SPI_SCK, ADC_IN5
    GP_A6 = GPIO_GPIO(0, 6),//SPI_MISO, TIM3_CH1, TIM1_BKIN, TIM16_CH1, EVENT_OUT, ADC_IN6
    GP_A7 = GPIO_GPIO(0, 7),//SPI_MOSI, TIM3_CH2, TIM14_CH1, TIM1_CH1N, TIM17_CH1, EVENTOUT, ADC_IN7
    GP_A9 = GPIO_GPIO(0, 9),//UART_TX, TIM1_CH2, 
    GP_A10 = GPIO_GPIO(0, 10),//UART_RX, TIM1_CH3, TIM17_BKIN
    GP_A13 = GPIO_GPIO(0, 13), //SWDIO - debug interface, can be used after uart debug implementation, IR_OUT
    GP_A14 = GPIO_GPIO(0, 14), //SWDCLK - debug interface, can be used after uart debug implementation, UART_TX
    GP_B1 = GPIO_GPIO(1, 1) //TIM3_CH4, TIM14_CH1, TIM1_CH3N, ADC_IN9
    //PF0, XTAL - used for external clock
    //PF1, XTAL - used for external clock
} gpio_t;

typedef enum
{
    GPIO_DIRECTION_IN = 0,
    GPIO_DIRECTION_OUT
} gpio_direction_t;

typedef enum
{
    GPIO_STATE_LOW,
    GPIO_STATE_HIGH
} gpio_state_t;

typedef enum
{
    GPIO_OUTPUT_TYPE_PUSH_PULL = 0,
    GPIO_OUTPUT_TYPE_OPEN_DRAIN = 1
} gpio_output_type_t;

typedef enum
{
    MUX_FUNC_GPIO_IN        = 0x00,
    MUX_FUNC_GPIO_OUT       = 0x10,
    
    MUX_FUNC0               = 0x20,
    MUX_FUNC_EVENTOUT       = MUX_FUNC0, //PA1
    MUX_FUNC_SPI            = MUX_FUNC0, //PA4, PA5, PA6, PA7
    MUX_FUNC_TIM17_BKIN     = MUX_FUNC0, //PA10
    MUX_FUNC_SWD            = MUX_FUNC0, //PA13, PA14
    MUX_FUNC_TIM14_CH1      = MUX_FUNC0, //PB1
    
    MUX_FUNC1,
    MUX_FUNC_UART           = MUX_FUNC1, //PA0, PA1, PA2, PA3, PA4, PA9, PA10, PA14
    MUX_FUNC_TIM3           = MUX_FUNC1, //PA6, PA7, PB1
    MUX_FUNC_IR_OUT         = MUX_FUNC1, //PA13
    
    MUX_FUNC2,
    MUX_FUNC_TIM1           = MUX_FUNC2, //PA6, PA7, PA9, PA10, PB1
    
    MUX_FUNC3, //not supported on stm32f030f4
    
    MUX_FUNC4,
    MUX_FUNC_TIM14_CH1_2    = MUX_FUNC4, //PA4, PA7
    MUX_FUNC_I2C            = MUX_FUNC4, //PA9, PA10
    
    MUX_FUNC5,
    MUX_FUNC_TIM16_CH1      = MUX_FUNC5, //PA6
    MUX_FUNC_TIM17_CH1      = MUX_FUNC5, //PA7
    
    MUX_FUNC6,
    MUX_FUNC_EVENOUT_2      = MUX_FUNC6, //PA6, PA7
    
    MUX_FUNC_ANALOG         = 0x30
} mux_function_t;

typedef enum
{
    GPIO_SPEED_LOW = 0x00,
    GPIO_SPEED_MEDIUM = 0x01,
    GPIO_SPEED_HIGH = 0x11
}gpio_speed_t;

void mux_set(gpio_t gpio, mux_function_t func);
void gpio_set_speed(gpio_t gpio, gpio_speed_t speed);

void gpio_init(gpio_t gpio, gpio_direction_t dir);
void gpio_set(gpio_t gpio, gpio_state_t state);
gpio_state_t gpio_get(gpio_t gpio);
void gpio_set_pullup(gpio_t gpio, gpio_state_t state);
void gpio_clear_pullup(gpio_t gpio);
void gpio_set_output_type(gpio_t gpio, gpio_output_type_t output_type);
#endif //_GPIO_H_
