/**
  ******************************************************************************
  * @file    board_led.c
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    16-October-2017
  * @brief   APUS Board LED control Source File.
  ******************************************************************************
  */

#include "stm32f0xx_gpio.h"
#include "board_led.h"

/* GPIO Pin identifier */
typedef struct _GPIO_PIN 
{
    GPIO_TypeDef *port;
    uint16_t      pin;
} GPIO_PIN;

/* LED GPIO Pins */
static const GPIO_PIN LED_PIN[] = 
{
    { GPIOC, GPIO_Pin_0 },
    { GPIOC, GPIO_Pin_1 },
    { GPIOC, GPIO_Pin_2 },
    { GPIOC, GPIO_Pin_6 },
    { GPIOC, GPIO_Pin_7 },
    { GPIOC, GPIO_Pin_8 }
};

/**
  * @brief  Initialize LEDs including GPIO ports.
  * @param  None
  * @retval None
*/
void LED_Initialize(void) 
{
    GPIO_InitTypeDef initStruct;
    uint32_t idx = 0;

    /* GPIO Ports Clock Enable */
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    /* initialize the GPIO struct */
    GPIO_StructInit(&initStruct);
    
    for (idx = 0; idx < sizeof(LED_PIN) / sizeof(GPIO_PIN); idx++)
    {
      /* disable the LEDs before activating output driver */
      GPIO_SetBits(LED_PIN[idx].port, LED_PIN[idx].pin);

      /* Configure GPIO pin */
      initStruct.GPIO_Pin = LED_PIN[idx].pin;
      initStruct.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_Init(LED_PIN[idx].port, &initStruct);
      
    }
}

typedef struct _GPIO_PIN_AF
{
    GPIO_TypeDef *port;
    uint16_t      pin;
    uint8_t       AF_PINSOURCE;
    uint8_t       AF;
} GPIO_PIN_AF;

/* LED GPIO Pins */
static const GPIO_PIN_AF RGB_LED_PIN[] = 
{
    { GPIOC, GPIO_Pin_8,GPIO_PinSource8, GPIO_AF_0 }
};


/**
  * @brief  Initialize RGB LEDs including GPIO ports.
  * @param  None
  * @retval None
*/
void RGB_LED_Initialize_PWM(void) 
{
    GPIO_InitTypeDef initStruct;
    uint32_t idx = 0;

    /* GPIO Ports Clock Enable */
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    /* initialize the GPIO struct */
    GPIO_StructInit(&initStruct);
    
    for (idx = 0; idx < sizeof(RGB_LED_PIN) / sizeof(GPIO_PIN); idx++)
    {
      /* disable the LEDs before activating output driver */
      GPIO_SetBits(RGB_LED_PIN[idx].port, RGB_LED_PIN[idx].pin);

      /* Configure GPIO pin */
      initStruct.GPIO_Pin = RGB_LED_PIN[idx].pin;
      initStruct.GPIO_Mode = GPIO_Mode_AF;
      GPIO_Init(RGB_LED_PIN[idx].port, &initStruct);
      
      // Set the Alternate Function Source for each Pin
      GPIO_PinAFConfig(RGB_LED_PIN[idx].port,RGB_LED_PIN[idx].AF_PINSOURCE,RGB_LED_PIN[idx].AF);
    }
}


/**
  * @brief  Turn on requested LED.
  * @param  num: LED number to switch on.
  * @retval None
*/
void LED_On(uint32_t num) 
{
    GPIO_WriteBit(LED_PIN[num].port, LED_PIN[num].pin, Bit_RESET);
}

/**
  * @brief  Turn off requested LED.
  * @param  num: LED number to switch off.
  * @retval None
*/
void LED_Off(uint32_t num) 
{
    GPIO_WriteBit(LED_PIN[num].port, LED_PIN[num].pin, Bit_SET);
}


/**
  * @brief  Write value to LEDs.
  * @param  val: Calue to be displayed on LEDs.
  * @retval None
*/
void LED_SetOut(uint32_t val) 
{
  uint32_t n;

    for (n = 0; n < LED_COUNT; n++) 
    {
        if (val & (1 << n)) 
        {
            LED_On(n);
        }
        else
        {
            LED_Off(n);
        }
    }
}

/**
  * @brief  Toggle requested LED.
  * @param  num: LED number to switch off.
  * @retval None
*/
void LED_Toggle(uint32_t num)
{
    if (LED_Get(num))
    {
        LED_Off(num);
    }
    else 
    {
        LED_On(num);
    }
}

/**
  * @brief  Get number of LEDs.
  * @param  None
  * @retval Number of available LEDs
*/
uint32_t LED_GetCount(void) 
{
    return LED_COUNT;
}

/**
  * @brief  Get state of requested LED.
  * @param  num: LED number.
  * @retval State of requested LEDs.
*/
int32_t LED_Get(uint32_t num) 
{
    if (GPIO_ReadOutputDataBit(LED_PIN[num].port, LED_PIN[num].pin))
    {
        return Bit_RESET;
    }
    return Bit_SET;
}
