/**
  ******************************************************************************
  * @file    rgb_timer.h
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    16-October-2017
  * @brief   APUS Board button control Header File.
  ******************************************************************************
  */

#ifndef __TIMER_IT_H
#define __TIMER_IT_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f0xx_tim.h"
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/

#define ARR_MAX_VALUE       0xFFFF  // Max Auto Reload Register Value for 16 Bit Timer

void Timer_IT_Initialize(const uint32_t Interrupt_frequency);

/* Exported functions ------------------------------------------------------- */


#endif /* __TIMER_IT_H */
