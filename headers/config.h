#ifndef _CONFIG_H_
    #define _CONFIG_H_

#include "common_macros.h"

//CPU
#define CPU_SPEED CPU_SPEED_48MHZ
#define CLOCK_SOURCE CLOCK_SOURCE_EXTERNAL

//MOTOR
#define GP_EN       GP_A4 //common
#define GP_DIR1     GP_A2 //motor 1
#define GP_STEP1    GP_B1 //motor 1


//UART
#define BAUD        115200
#define TX_PIN      GP_A9
#define RX_PIN      GP_A10
#define STOP_BITS   UART_STOP_1BIT
#define PARITY      UART_PARITY_NONE
#define DATA_BITS   UART_DATA_BITS_8

//DEBUG
#define DEBUG_MODE  DEBUG_MODE_UART

//SYSTEM TIMER
#define TICK_TIMEOUT 1000 //1ms


//BUTTONS
#define BTN_END_1   GP_A0
#define BTN_END_2   GP_A1

#define BTN_UP      GP_A3
#define BTN_DOWN    GP_A5
#define BTN_START   GP_A6

//SHUTTER
#define SHUTTER_RELEASE GP_A14 // shared with swd 

#endif //_CONFIG_H_
