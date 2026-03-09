/**
  ******************************************************************************
  * @file    rgb_timer.h
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    16-October-2017
  * @brief   APUS Board button control Header File.
  ******************************************************************************
  */

#ifndef __RGB_TIMER_H
#define __RGB_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f0xx_tim.h"
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/



/* Exported functions ------------------------------------------------------- */


#define ARR_MAX_VALUE 0xFFFF
#define MAX_DUTY_CYCLE 100

/**
 * @brief Function that initializes the Timer Channel for the RGB LED
 * 
 * @param LEDBlinkFrequency Frequency of the Blink Sequence
 * @param LEDDutyCycle  Duty Cycle of the Blink Sequence
 */
void RGB_Timer_Initialize(const uint32_t LEDBlinkFrequency, const uint16_t LEDDutyCycle);

/**
 * @brief Function that Changes the blink sequence
 * 
 * @param LEDBlinkFrequency Frequency of the Blink Sequence
 * @param LEDDutyCycle  Duty Cycle of the Blink Sequence
 */
void RGB_Timer_ChangeInterval(const uint32_t LEDBlinkFrequency, const uint16_t LEDDutyCycle);

#endif /* __RGB_TIMER_H */
