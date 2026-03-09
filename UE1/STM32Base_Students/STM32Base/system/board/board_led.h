/**
  ******************************************************************************
  * @file    board_led.h
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    16-October-2017
  * @brief   APUS Board LED control Header File.
  ******************************************************************************
  */

#ifndef __BOARD_LED_H
#define __BOARD_LED_H

#include <stdint.h>

#define LED3    0
#define LED4    1
#define LED5    2
#define LED6_RED      3
#define LED6_GREEN    4
#define LED6_BLUE     5

#define LED3_MASK    (1<<LED3)
#define LED4_MASK    (1<<LED4)
#define LED5_MASK    (1<<LED5)
#define LED6_RED_MASK     (1<<LED6_RED)
#define LED6_GREEN_MASK   (1<<LED6_GREEN)
#define LED6_BLUE_MASK    (1<<LED6_BLUE)

#define LED_COUNT 6

void RGB_LED_Initialize_PWM(void);

/**
  * @brief  Initialize LEDs including GPIO ports.
  * @param  None
  * @retval None
*/
void LED_Initialize(void);

/**
  * @brief  Turn on requested LED.
  * @param  num: LED number to switch on.
  * @retval None
*/
void LED_On(uint32_t num);

/**
  * @brief  Turn off requested LED.
  * @param  num: LED number to switch off.
  * @retval None
*/
void LED_Off(uint32_t num);

/**
  * @brief  Write value to LEDs.
  * @param  val: Calue to be displayed on LEDs.
  * @retval None
*/
void LED_SetOut(uint32_t val);

/**
  * @brief  Toggle requested LED.
  * @param  num: LED number to switch off.
  * @retval None
*/
void LED_Toggle(uint32_t num);

/**
  * @brief  Get number of LEDs.
  * @param  None
  * @retval Number of available LEDs
*/
uint32_t LED_GetCount(void);

/**
  * @brief  Get state of requested LED.
  * @param  num: LED number.
  * @retval State of requested LEDs.
*/
int32_t LED_Get(uint32_t num);

#endif /* __BOARD_LED_H */
