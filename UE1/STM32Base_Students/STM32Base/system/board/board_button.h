/**
  ******************************************************************************
  * @file    board_button.h
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    16-October-2017
  * @brief   APUS Board button control Header File.
  ******************************************************************************
  */

#ifndef __BOARD_BUTTON_H
#define __BOARD_BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
#define BUTTON_USER0_MASK       (1<<0)
#define BUTTON_USER1_MASK       (1<<1)
#define BUTTON_WAKEUP_MASK      (1<<2)


/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize buttons including GPIO ports.
  * @param  None
  * @retval None
*/
void Button_Initialize(void);

/**
  * @brief  Get buttons state.
  * @param  None
  * @retval The state of pressed buttons.
*/
uint32_t Button_GetState(void);

#endif /* __BOARD_BUTTON_H */
