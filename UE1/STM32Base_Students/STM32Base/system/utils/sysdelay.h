/**
  ******************************************************************************
  * @file    sysdelay.h
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    16-October-2017
  * @brief   SysDelay Header File.
  ******************************************************************************
  */

#ifndef __SYS_DELAY_H
#define __SYS_DELAY_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * @brief  This function is called to increment a global variable "uwTick"
  *         used as application time base.
  * @note   In the default implementation, this variable is incremented each 1ms
  *         in Systick ISR.
  * @note   This function is declared as __weak to be overwritten in case of other 
  *         implementations in user file.
  * @retval None
*/
void SysDelay_IncTicks(void);

/**
  * @brief  Provides a tick value in millisecond.
  * @retval tick value
  */
uint32_t SysDelay_GetTicks(void);

/**
  * @brief  This function provides accurate delay (in milliseconds) based 
  *         on variable incremented.
  * @param  Delay: specifies the delay time length, in milliseconds.
  * @retval None
  */
void SysDelay_Delay(volatile uint32_t Delay_ms);

#endif /* __SYS_DELAY_H */
