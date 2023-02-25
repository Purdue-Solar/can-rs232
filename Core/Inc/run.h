/**
 * @file run.h
 * @author Purdue Solar Racing (Aidan Orr)
 * @brief
 * @version 0.1
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

// STM32 standard includes
#ifdef BOARD_STM32
#include "stm32_includer.h"
#include STM32_INCLUDE(BOARD_STM32, hal.h)
#include STM32_INCLUDE(BOARD_STM32, hal_def.h)
#include STM32_INCLUDE(BOARD_STM32, hal_uart.h)
#include STM32_INCLUDE(BOARD_STM32, hal_can.h)
#endif // BOARD_STM32

// C standard includes
#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#include <stdbool.h>	// stdbool.h unnecessary for C++
#endif // __cplusplus

// Use C-linkage to allow this function to be called from C code.
//  - Ex: STM32 Cube auto generates main.c which only executes C code, 
//        this allows the function to be called from the main function, 
//        while still executing C++ code
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/**
 * @brief Entrypoint for user code, intended to be run after the board initializes all of the peripherals
 *
 * @param UART_HandleTypeDef* uart Reference to system UART object
 */
void run(UART_HandleTypeDef* uart, CAN_HandleTypeDef* can);

#ifdef __cplusplus
}
#endif // __cplusplus
