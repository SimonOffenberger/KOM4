/**
  ******************************************************************************
  * @file    board_button.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    14-October-2025
  * @brief   APUS Board button control Source File.
  ******************************************************************************
  */



/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_gpio.h"
#include "board_button.h"


/* GPIO Pin identifier */
typedef struct _GPIO_PIN 
{
    GPIO_TypeDef *port;
    uint16_t      pin;
} GPIO_PIN;

/* Button GPIO Pins */
static const GPIO_PIN BUTTON_PIN[] = 
{
    { GPIOD, GPIO_Pin_0 },	// User Button 0
    { GPIOD, GPIO_Pin_1 },	// User Button 1
    { GPIOA, GPIO_Pin_0 }		// Wakeup Button
};

/**
  * @brief  Initialize Buttons including GPIO ports.
  * @param  None
  * @retval None
*/
void Button_Initialize(void) 
{
    GPIO_InitTypeDef initStruct;
    uint32_t idx = 0;

    /* GPIO Ports Clock Enable */
    RCC->AHBENR |= RCC_AHBENR_GPIODEN;	// Clock for User Button
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;	// Clock for Wakeup Button
    /* initialize the GPIO struct */
    GPIO_StructInit(&initStruct);
    
    for (idx = 0; idx < sizeof(BUTTON_PIN) / sizeof(GPIO_PIN); idx++)
    {

      /* Configure GPIO pin */
      initStruct.GPIO_Pin = BUTTON_PIN[idx].pin;
      initStruct.GPIO_Mode = GPIO_Mode_IN;
      initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(BUTTON_PIN[idx].port, &initStruct);
    }
}

/**
  * @brief  Function that returns bitwise the state of a Button 
  * @param  None
  * @retval uint32_t Bitwise state of each button
*/
uint32_t Button_GetState(void){
  uint32_t button_States =0;
  uint32_t idx = 0;
  
  // loop over all the defined buttons
  for (idx = 0; idx < sizeof(BUTTON_PIN) / sizeof(GPIO_PIN); idx++)
  {
    // read the ID-Register and shift the corrospondig bit to the correct position in button_states
    button_States |= (((BUTTON_PIN[idx].port->IDR & BUTTON_PIN[idx].pin) == BUTTON_PIN[idx].pin) << idx);
  }
  
  return button_States;
}


