/**
 * @file board_adc.h
 * @author Simon Offenberger
 * @brief Module for initializing and using the ADC peripheral
 * @version 1.0
 * @date 2025-12-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __BOARD_ADC_H
#define __BOARD_ADC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0xx_adc.h"

// Error codes
#define ERROR_ADC_DATA_NOT_READY -1

#define ADC_CHSEL_CLEAR_MASK 0x7FFFF

/**
 * @brief Initialize the ADC with the specified channels
 *
 * @param channels Bitmask of ADC channels to initialize
 */
void ADC_Initialize(const uint32_t channels);

/**
 * @brief  Get the temperature sensor value in degree Celsius
 * @retval Temperature sensor value in degree Celsius
 * @note This Function is from the STM32F0xx Reference Manual under
 *       A.7.16 : Temperature computation code example!
 *
 */
int32_t ADC_GET_int_Temperatur();

#endif /* __BOARD_ADC_H */