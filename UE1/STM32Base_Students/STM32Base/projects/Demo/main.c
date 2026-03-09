/**
  ******************************************************************************
  * @file    main.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    14-October-2025
  * @brief   This file Contains the main Function which calles the current poject main
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_conf.h"
#include "sysdelay.h"
#include "3bit_cnt.h"
#include "alarm.h"
#include "LedTimer.h"
#include "Interrupt_Proj.h"
#include "Uart_Vcom.h"
#include "Uart_DMA.h"
#include "ADC_Temp_prj.h"
#include "i2c_temp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the sys tick timer (M0 core) which is used for delay.
  * @param  None
  * @retval None
  */
void SysTick_Init(void) 
{
    /* init the sys tick timer to be called every 1ms */
    SysTick_Config(SystemCoreClock / 1000);
} 

/**
  * @brief  This is the SysTick interrupt handler which is called every 1ms. 
            We have to increment the HAL tick counter which is used for SysDelay
  * @param  None
  * @retval None
  */
void SysTick_Handler()
{
    SysDelay_IncTicks();
}

/**
* @brief  Main program
* @param  None
* @retval None
*/
int main(void)
{ 
    /* init the sys tick counter (needed for SysDelay) */
    SysTick_Init();

    // switch to project I2C Temperature Sensor
    main_I2C_Temp();
}
