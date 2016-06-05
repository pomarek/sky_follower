#ifndef _CONFIG_H_
    #define _CONFIG_H_

#include "common_macros.h"

//CPU
#define CPU_SPEED CPU_SPEED_48MHZ
#define CLOCK_SOURCE CLOCK_SOURCE_EXTERNAL

//MOTOR
#define GP_EN GP_A4


//UART
#define BAUD        9600
#define TX_PIN      GP_A9
#define RX_PIN      GP_A10
#define STOP_BITS   UART_STOP_1BIT
#define PARITY      UART_PARITY_NONE
#define DATA_BITS   UART_DATA_BITS_8

//DEBUG
#define DEBUG_MODE  DEBUG_MODE_UART

#endif //_CONFIG_H_
