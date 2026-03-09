/**
 * @file board_adc.c
 * @author Simon Offenberger
 * @brief Module for initializing and using the ADC peripheral
 * @version 1.0
 * @date 2025-12-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "board_adc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_rcc.h"

/**
 * @brief Initialize the ADC with the specified channels
 *
 * @param channels Bitmask of ADC channels to initialize
 * @return int Status of the initialization (0 for success, error code otherwise)
 */
void ADC_Initialize(const uint32_t channels)
{
  // Enable Clock for ADC1
  RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE);

  // Disable [ Analog Watchdog, Discontinuous mode, Disable Continous Mode, Auto off, Wait mode,] 12-bit resolution
  ADC1->CFGR1 &= ~(ADC_CFGR1_AWDEN | ADC_CFGR1_DISCEN | ADC_CFGR1_CONT | ADC_CFGR1_AUTOFF | ADC_CFGR1_WAIT | ADC_CFGR1_RES);

  // Set Clock to PCLK/4 -> PCLK is 48MHz, ADC Clock is 12MHz
  // OK Maximum ADC clock frequency is 14MHz -> STM32F072vb Datasheet
  ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE;
  ADC1->CFGR2 |= (ADC_CFGR2_CKMODE_1);

  // Set Sampling Time for Temperature Sensor Channel 111 cycles
  // This is required for temperature sensor reading accuracy -> STM32F0xx Reference Manual
  // can be written because only content is Sampling Rate
  ADC1->SMPR = (ADC_SMPR1_SMPR);

  // can be written because only content is Channel Selection
  ADC1->CHSELR = (channels &  ADC_CHSEL_CLEAR_MASK); // Set ADC Channels

  // if Temperature Sensor channel is selected, enable the Temperature Sensor
  if (channels & ADC_Channel_TempSensor)
  {
    // Enable Temperature Sensor Channel
    ADC->CCR |= ADC_CCR_TSEN;
  }

  // calibrate ADC
  // start calibration
  /**< Note: This Register is Marked with RS -> Read/Set -> 
   * Software can read as well as set this bit. Writing '0' has no effect on the bit value. */
  ADC1->CR = ADC_CR_ADCAL;

  // wait until calibration is complete
  while ((ADC1->CR & ADC_CR_ADCAL) != 0);

  // Enable ADC
  ADC1->CR = ADC_CR_ADEN; // Enable ADC

  while ((ADC1->ISR & ADC_ISR_ADRDY) == 0); // Wait until ADC is ready
}

/* Temperature sensor calibration value address */
#define TEMP110_CAL_ADDR ((uint16_t *)((uint32_t)0x1FFFF7C2))
#define TEMP30_CAL_ADDR ((uint16_t *)((uint32_t)0x1FFFF7B8))
#define TEMP_CAL_HIGH 110
#define TEMP_CAL_LOW 30
#define VDD_CALIB ((uint16_t)(330))
#define VDD_APPLI ((uint16_t)(330))

/**
 * @brief  Get the temperature sensor value in degree Celsius
 * @retval Temperature sensor value in degree Celsius
 * @note This Function is from the STM32F0xx Reference Manual under
 *       A.7.16 : Temperature computation code example!
 *
 */
int32_t ADC_GET_int_Temperatur()
{
  int32_t temperature; /* will contain the temperature in degrees Celsius */

  // ADC is only ready if it was enabled previously
  if (ADC1->ISR & ADC_ISR_ADRDY)
  {
    // clear ready bit
    /**< Note: This Register is Marked with r_wc1 -> read/clear(rc_w1) -> 
    * Software can read as well as clear this bit by writing 1. Writing '0' has no effect on the bit value. */
    ADC1->ISR = ADC_ISR_ADRDY;

    // Start a new conversion
    ADC1->CR = ADC_CR_ADSTART;

    // inform that no Data is Ready Yet
    return ERROR_ADC_DATA_NOT_READY;
  }

  // if the conversion is not completed return
  if (!(ADC1->ISR & ADC_ISR_EOC))
    return ERROR_ADC_DATA_NOT_READY;

  // Calculation for Degree resolution in temperature reading
  temperature = (((int32_t)ADC1->DR * VDD_APPLI / VDD_CALIB) - (int32_t)*TEMP30_CAL_ADDR);
  temperature = temperature * (int32_t)(TEMP_CAL_HIGH - TEMP_CAL_LOW);
  temperature = temperature / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
  temperature = temperature + TEMP_CAL_LOW;

  // Clear End of Conversion flag & End of Sequence & End of Sampling flags
  // must be written because it is cleard by writing 1 to it
  // by reading first -> all the active flags would be cleared!
  ADC1->ISR = ADC_ISR_EOC | ADC_ISR_EOSEQ | ADC_ISR_EOSMP;

  // Start next conversion
  // must also be written no read modify write !!
  ADC1->CR = ADC_CR_ADSTART;

  return temperature;
}