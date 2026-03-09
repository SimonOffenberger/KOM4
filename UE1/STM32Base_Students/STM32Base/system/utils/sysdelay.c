/**
  ******************************************************************************
  * @file    sysdelay.c
  * @author  Andreas Oyrer
  * @version V1.1
  * @date    02-October-2022
  * @brief   SysDelay Source File.
  ******************************************************************************
  */

#include "sysdelay.h"

static volatile uint32_t uwTick = 0;

/**
  * @brief  This function is called to increment a global variable "uwTick"
  *         used as application time base.
  * @note   In the default implementation, this variable is incremented each 1ms
  *         in Systick ISR.
  * @note   This function is declared as __weak to be overwritten in case of other 
  *         implementations in user file.
  * @retval None
*/
void SysDelay_IncTicks(void)
{
    uwTick++;
}

/**
  * @brief  Provides a tick value in millisecond.
  * @retval tick value
  */
uint32_t SysDelay_GetTicks(void)
{
    return uwTick;
}

/**
  * @brief  This function provides accurate delay (in milliseconds) based 
  *         on variable incremented.
  * @param  Delay: specifies the delay time length, in milliseconds.
  * @retval None
  */
void SysDelay_Delay(volatile uint32_t Delay_ms)
{
    uint32_t tickstart = 0;
    
    tickstart = SysDelay_GetTicks();
    // 32 bit overflow works!
    while ((SysDelay_GetTicks() - tickstart) < Delay_ms)
    {
    }
}
